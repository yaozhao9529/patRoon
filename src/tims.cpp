#include <cstdint>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "opentims++/opentims_all.h"

#include <Rcpp.h>

#include "utils.h"

namespace {

struct SpectrumIMS // UNDONE: merge with other struct(s)
{
    // NOTE: IDs is scan ID for frame data or unique peak identifier for (collapsed) spectra
    std::vector<uint32_t> IDs, intensities;
    std::vector<double> mzs, mobilities;
    
    SpectrumIMS(size_t size) : IDs(size), intensities(size), mzs(size), mobilities(size) { }
    SpectrumIMS() = default;
    
    void addData(unsigned id, double mz, unsigned inten, double mob)
    {
        IDs.push_back(id);
        mzs.push_back(mz);
        intensities.push_back(inten);
        mobilities.push_back(mob);
    }
    void addData(double mz, unsigned inten, double mob)
    {
        addData(IDs.size(), mz, inten, mob);
    }
    void addData(const SpectrumIMS &sp)
    {
        IDs.insert(IDs.end(), sp.IDs.begin(), sp.IDs.end());
        mzs.insert(mzs.end(), sp.mzs.begin(), sp.mzs.end());
        intensities.insert(intensities.end(), sp.intensities.begin(), sp.intensities.end());
        mobilities.insert(mobilities.end(), sp.mobilities.begin(), sp.mobilities.end());
    }
    void setData(size_t i, unsigned id, double mz, unsigned inten, double mob)
    { IDs[i] = id; mzs[i] = mz; intensities[i] = inten; mobilities[i] = mob; }
    void resize(size_t s)
    {
        IDs.resize(s);
        mzs.resize(s);
        intensities.resize(s);
        mobilities.resize(s);
    }
    size_t size(void) const { return IDs.size(); }
    bool empty(void) const {return IDs.empty(); }
    void clear(void)
    {
        IDs.clear();
        intensities.clear();
        mzs.clear();
        mobilities.clear();
    }
};

class IMSFrame
{
    std::vector<SpectrumIMS> spectra;
    std::vector<unsigned> scanIDs;
    std::vector<double> mobilities;
    
public:
    IMSFrame(size_t size) : spectra(size), scanIDs(size), mobilities(size) { }
    IMSFrame() = default;

    bool empty(void) const { return spectra.empty(); }
    size_t size(void) const { return spectra.size(); }

    SpectrumIMS &operator[](size_t i) { return spectra[i]; }
    const SpectrumIMS &operator[](size_t i) const { return spectra[i]; }
    auto &getSpectra(void) { return spectra; }
    const auto &getSpectra(void) const { return spectra; }

    const auto &getMobilities(void) const { return mobilities; }

    void addSpectrum(unsigned sID, double mob, SpectrumIMS &&spec)
    {
        // Rcpp::Rcout << "Add spec: " << sID << "/" << mob << "/" << spec.size() << "\n";
        scanIDs.push_back(sID);
        mobilities.push_back(mob);
        spectra.push_back(spec);
    }
};

struct SpectrumFilterParams
{
    unsigned minIntensity;
    std::pair<double, double> mzRange, mobilityRange;

    SpectrumFilterParams(unsigned minInt, double mzStart, double mzEnd, double mobStart, double mobEnd) :
        minIntensity(minInt), mzRange(mzStart, mzEnd), mobilityRange(mobStart, mobEnd) { }
    SpectrumFilterParams() = default;
    bool unset(void) const
    {
        return (minIntensity == 0 && mzRange == std::make_pair(0.0, 0.0) && mobilityRange == std::make_pair(0.0, 0.0));
    }
};

struct SpectrumScanRanges
{
    std::vector<double> scanStarts, scanEnds;
    SpectrumScanRanges(const std::vector<double> &ss, const std::vector<double> &se) : scanStarts(ss), scanEnds(se) { }
    SpectrumScanRanges() = default;
    bool empty(void) const { return (scanStarts.empty()); }
}


SpectrumIMS spectrumTopMost(const SpectrumIMS &spec, unsigned topMost, double precursorMZ = 0.0)
{
    if (topMost == 0 || spec.size() <= topMost)
        return spec;
    
    SpectrumIMS specFiltered(topMost);
    const auto inds = getSortedInds(spec.intensities);
    //auto indsSet = std::set<size_t>(std::advance(inds.begin(), inds.size() - topMost), inds.end());
    for (size_t i=0; i<topMost; ++i)
    {
        const auto j = inds[(inds.size()-1) - i];
        specFiltered.setData(i, spec.IDs[j], spec.mzs[j], spec.intensities[j], spec.mobilities[j]);
    }

    if (precursorMZ != 0.0) // make sure precursor is also in there
    {
        double closestDiff = -1.0;
        size_t closestInd;
        for (size_t i = 0; i < spec.mzs.size(); ++i)
        {
            const double d = std::abs(spec.mzs[i] - precursorMZ);
            // UNDONE: configrable range? (same for R's getMZIndexFromMSPeakList())
            if ((closestDiff < 0.0 || d < closestDiff) && numberLTE(spec.mzs[i], precursorMZ, 0.01))
            {
                closestDiff = d;
                closestInd = i;
            }
        }
        if (closestDiff >= 0.0) // found precursor?
        {
            if (std::find(std::next(inds.cbegin(), inds.size() - topMost), inds.cend(), closestInd) == inds.cend())
                specFiltered.addData(spec.mzs[closestInd], spec.intensities[closestInd], spec.mobilities[closestInd]);
        }
    }

    return specFiltered;
}

auto getTIMSDecompBuffers(size_t size)
{
    // get buffers for decompression (see TimsDataHandle::extract_frames())
    return std::make_pair(std::unique_ptr<ZSTD_DCtx, decltype(&ZSTD_freeDCtx)>(ZSTD_createDCtx(), &ZSTD_freeDCtx),
                          std::make_unique<char[]>(size));
}

// From http://mochan.info/c++/2019/06/12/returntype-deduction.html
using TIMSDecompBufferPair = decltype(getTIMSDecompBuffers(std::declval<size_t>()));

IMSFrame getIMSFrame(TimsFrame &tframe, const TIMSDecompBufferPair &bufs, const SpectrumFilterParams &filtParams = {},
                     const SpectrumScanRanges &scanRanges = { },
                     unsigned topMost = 0, double precursorMZ = -1.0, bool onlyWithPrecursor = false,
                     bool pruneEmptySpectra = false)

{
    if (tframe.num_peaks == 0)
        return IMSFrame();
    
    /* The following assumptions are made:
        1. scans are sorted from low to high
        2. mobilities are equal for each scan and sorted from high to low
        3. m/z values are sorted from low to high in each scan (not used atm)
    */

    tframe.decompress(bufs.second.get(), bufs.first.get());
    std::vector<uint32_t> IDs(tframe.num_peaks), intensities(tframe.num_peaks);
    std::vector<double> mzs(tframe.num_peaks), mobilities(tframe.num_peaks);
    tframe.save_to_buffs(nullptr, IDs.data(), nullptr, intensities.data(), mzs.data(), mobilities.data(), nullptr);
    tframe.close();

    IMSFrame ret;
    SpectrumIMS curSpec;
    unsigned curScanID = 0;
    double curMob = 0.0;
    const auto len = IDs.size();
    const bool subsetScans = !scanRanges.empty();
    const auto latestScan = (subsetScans) ? *(std::max_element(scanRanges.scanEnds.begin(), scanRanges.scanEnds.end())) : 0;

    for (size_t i=0; ; ++i)
    {
        // new/last scan or m/z is already out of upper range?
        if (i >= len || curScanID != IDs[i])
        {
            bool doAddSpec = !curSpec.empty();
            if (doAddSpec && onlyWithPrecursor && precursorMZ >= 0.0)
                doAddSpec = std::any_of(curSpec.mzs.begin(), curSpec.mzs.end(), [&](double mz)
                                    { return compareTol(mz, precursorMZ, 0.01); });
            if (doAddSpec && topMost != 0)
            {
                curSpec = spectrumTopMost(curSpec, topMost, precursorMZ);
                doAddSpec = !curSpec.empty();
            }
            if (doAddSpec)
                ret.addSpectrum(curScanID, curMob, std::move(curSpec));
            // add empty spectrum if current is invalid, we want to keep empty spectra and if we're not initializing
            // the loop (i!=0)
            else if (!pruneEmptySpectra && i != 0)
                ret.addSpectrum(curScanID, curMob, SpectrumIMS());
            
            if (i >= len)
                break; // done

            curSpec.clear();
            curScanID = IDs[i];
            curMob = mobilities[i];

            if (filtParams.mobilityRange.first != 0.0 && curMob < filtParams.mobilityRange.first)
                break; // no need to go further, next scans all have lower mobility
            if (subsetScans && curScanID > latestScan)
                break; // no need to go further, next scans all have higher IDs

            bool inrange = true;
            if (subsetScans)
            {
                inrange = false;
                for (size_t j = 0; j < scanRanges.scanStarts.size() && !inrange; ++j)
                    inrange = (curScanID >= scanRanges.scanStarts[j] && curScanID <= scanRanges.scanEnds[j]);
            }
            if (inrange && filtParams.mobilityRange.second != 0.0)
                inrange = numberLTE(curMob, filtParams.mobilityRange.second, 1E-8);
            if (!inrange)
            {
                // try again with next scan: increment until last element (i will be incremented again in main for loop)
                for (; i < (len-1) && IDs[i+1] == curScanID; ++i)
                    ;
                // Rcpp::Rcout << "i: " << i << "/" << len << "\n";
                continue;
            }
        }

        if (filtParams.mzRange.first != 0.0 && mzs[i] < filtParams.mzRange.first)
            continue;
        if (filtParams.mzRange.second != 0.0 && mzs[i] > filtParams.mzRange.second)
            continue;
        if (filtParams.minIntensity != 0 && intensities[i] < filtParams.minIntensity)
            continue;
        
        curSpec.addData(mzs[i], intensities[i], mobilities[i]);
    }

    return ret;
}

template <typename... Args>
std::vector<IMSFrame> getTIMSFrames(TimsDataHandle &TDH, const std::vector<unsigned> &frameIDs,
                                    const SpectrumFilterParams &filtParams,
                                    const std::vector<SpectrumScanRanges> &scanRangesList, Args... args)
{
    std::vector<IMSFrame> ret(frameIDs.size());
    ThreadExceptionHandler exHandler;
    const std::vector<unsigned> emptyScanRange;

    // UNDONE: make num_threads configurable
    #pragma omp parallel num_threads(8)
    {
        auto TBuffers = getTIMSDecompBuffers(TDH.get_decomp_buffer_size());
    
        #pragma omp for
        for (size_t i=0; i<frameIDs.size(); ++i)
        {
            exHandler.run([&]
            {
                const auto fri = frameIDs[i];
                if (!TDH.has_frame(fri))
                    return;
                if (!scanRangesList.empty())
                    ret[i] = getIMSFrame(TDH.get_frame(fri), TBuffers, filtParams, scanRangesList[i], args...);
                else
                    ret[i] = getIMSFrame(TDH.get_frame(fri), TBuffers, filtParams, SpectrumScanRanges(), args...);
            });
        }
    }
    exHandler.reThrow();

    return ret;
}


SpectrumIMS filterSpectrum(const SpectrumIMS &spec, const SpectrumFilterParams &params)
{
    if (params.unset())
        return spec;
    
    SpectrumIMS specFiltered;
    for (size_t i=0; i<spec.size(); ++i)
    {
        if (params.mzRange.first != 0.0 && spec.mzs[i] < params.mzRange.first)
            continue;
        if (params.mzRange.second != 0.0 && spec.mzs[i] > params.mzRange.second)
            continue;
        if (params.mobilityRange.first != 0.0 && spec.mobilities[i] < params.mobilityRange.first)
            continue;
        if (params.mobilityRange.second != 0.0 && spec.mobilities[i] > params.mobilityRange.second)
            continue;
        if (params.minIntensity != 0 && spec.intensities[i] < params.minIntensity)
            continue;
        
        specFiltered.addData(spec.IDs[i], spec.mzs[i], spec.intensities[i], spec.mobilities[i]);
    }
    
    return specFiltered;
}

SpectrumIMS subsetSpectrum(const SpectrumIMS &spec, const std::vector<unsigned> &scanStarts,
                           const std::vector<unsigned> &scanEnds)
{
    SpectrumIMS specFiltered;
    for (size_t i=0; i<spec.size(); ++i)
    {
        for (size_t j=0; j<scanStarts.size(); ++j)
        {
            if (spec.IDs[i] >= scanStarts[j] && spec.IDs[i] <= scanEnds[j])
            {
                specFiltered.addData(spec.IDs[i], spec.mzs[i], spec.intensities[i], spec.mobilities[i]);
                break;
            }
        }
    }
    
    return specFiltered;
}

SpectrumIMS flattenSpectra(const std::vector<SpectrumIMS> &spectra)
{
    SpectrumIMS ret;
    for (const auto &sp : spectra)
        ret.addData(sp);
    return ret;
}

SpectrumIMS collapseIMSFrame(const SpectrumIMS &flattenedSpecs, clusterMethod method, double window,
                             unsigned minAbundance)
{
    if (flattenedSpecs.empty())
        return flattenedSpecs;

    const std::vector<int> clusts = clusterNums(flattenedSpecs.mzs, method, window);
    const int maxClust = *(std::max_element(clusts.begin(), clusts.end()));
    SpectrumIMS binnedSpectrum(maxClust + 1);
    std::vector<unsigned> binSizes(maxClust + 1);

    // Rcpp::Rcout << "flattenedSpecs: " << flattenedSpecs.size() << "\n";
    // Rcpp::Rcout << "maxClust: " << maxClust << "\n";

    // sum data for each cluster
    for (size_t i=0; i<clusts.size(); ++i)
    {
        const size_t cl = clusts[i];
        const double inten = static_cast<double>(flattenedSpecs.intensities[i]);
        binnedSpectrum.mzs[cl] += (flattenedSpecs.mzs[i] * inten);
        binnedSpectrum.mobilities[cl] += (flattenedSpecs.mobilities[i] * inten);
        binnedSpectrum.intensities[cl] += flattenedSpecs.intensities[i];
        ++binSizes[cl];
    }

    // average data
    for (size_t i=0; i<binnedSpectrum.size(); ++i)
    {
        const double inten = static_cast<double>(binnedSpectrum.intensities[i]);
        binnedSpectrum.mzs[i] /= inten;
        binnedSpectrum.mobilities[i] /= inten;
    }

    // Rcpp::Rcout << "binnedSpectrum: " << binnedSpectrum.size() << "\n";

    // sort spectrum && remove outliers
    const auto sortedInds = getSortedInds(binnedSpectrum.mzs);
    SpectrumIMS sortedSpectrum;
    for (size_t i=0; i<sortedInds.size(); ++i)
    {
        const auto j = sortedInds[i];
        if (binSizes[j] < minAbundance)
            continue;
        sortedSpectrum.addData(i+1, binnedSpectrum.mzs[j], binnedSpectrum.intensities[j],
                               binnedSpectrum.mobilities[j]);
    }
    
    // Rcpp::Rcout << "sortedSpectrum: " << sortedSpectrum.size() << "\n";

    return sortedSpectrum;
}

SpectrumIMS collapseIMSFrames(TimsDataHandle &TDH, const std::vector<unsigned> &frameIDs,
                              const SpectrumFilterParams &preFilterParams, const SpectrumFilterParams &postFilterParams,
                              unsigned topMost, clusterMethod method, double window,
                              unsigned minAbundance, const std::vector<std::vector<unsigned>> &scanStarts = { },
                              const std::vector<std::vector<unsigned>> &scanEnds = { })
{
    if (frameIDs.empty())
        return SpectrumIMS();
        
    const auto frames = getTIMSFrames(TDH, frameIDs, preFilterParams, topMost, -1.0, false, false, scanStarts, scanEnds);
    std::vector<SpectrumIMS> collapsedSpectra(frameIDs.size());
    
    // UNDONE: make num_threads configurable
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
        for (size_t i=0; i<frameIDs.size(); ++i)
            collapsedSpectra[i] = collapseIMSFrame(flattenSpectra(frames[i].getSpectra()), method, window, minAbundance));
    }
    
    // collapse result
    SpectrumIMS ret = collapseIMSFrame(flattenSpectra(collapsedSpectra), method, window, minAbundance);
    // average intensities
    for (auto &inten : ret.intensities)
        inten /= frameIDs.size();
    
    ret = filterSpectrum(ret, postFilterParams);
    
    return ret;
}

using EIX = std::pair<std::vector<double>, std::vector<unsigned>>; // used for EIC/EIM data

EIX sortCompressEIX(const EIX &eix)
{
    const auto ord = getSortedInds(eix.first);
    const auto len = ord.size();
    EIX ret;
    
    for (size_t j=0; j<len; ++j)
    {
        const auto ordInd = ord[j];
        if (j > 0 && j < (len-1) && eix.second[ordInd] == 0)
        {
            const auto prevInt = eix.second[ord[j-1]], nextInt = eix.second[ord[j+1]];
            if (prevInt == 0 && nextInt == 0)
                continue; // skip points with zero intensities that are neighbored by others.
            
        }
        ret.first.push_back(eix.first[ordInd]);
        ret.second.push_back(eix.second[ordInd]);
    }
    return ret;
}


}

// [[Rcpp::export]]
void initBrukerLibrary(const std::string &path)
{
    setup_bruker(path);
}

// [[Rcpp::export]]
Rcpp::DataFrame collapseTIMSFrame(const std::string &file, size_t frameID, const std::string &method, double mzWindow,
                                  double mzStart = 0.0, double mzEnd = 0.0, double mobilityStart = 0.0,
                                  double mobilityEnd = 0.0, unsigned minAbundance = 1, unsigned topMost = 0,
                                  unsigned minIntensity = 0,
                                  Rcpp::Nullable<Rcpp::IntegerVector> scanStartsN = R_NilValue,
                                  Rcpp::Nullable<Rcpp::IntegerVector> scanEndsN = R_NilValue, double precursorMZ = 0.0,
                                  bool onlyWithPrecursor = false, bool flatten = false)
{
    TimsDataHandle TDH(file);
    if (!TDH.has_frame(frameID))
        Rcpp::stop("Frame doesn't exist.");

    SpectrumFilterParams filterP(minIntensity, mzStart, mzEnd, mobilityStart, mobilityEnd);

    std::vector<unsigned> scanStarts, scanEnds;
    if (scanStartsN.isUsable())
        scanStarts = Rcpp::as<std::vector<unsigned>>(scanStartsN);
    if (scanEndsN.isUsable())
        scanEnds = Rcpp::as<std::vector<unsigned>>(scanEndsN);

    auto TBuffers = getTIMSDecompBuffers(TDH.get_decomp_buffer_size());
    auto frame = getIMSFrame(TDH.get_frame(frameID), TBuffers, filterP, SpectrumScanRanges(scanStarts, scanEnds),
                             topMost, precursorMZ, onlyWithPrecursor, false);
    const auto flsp = flattenSpectra(frame.getSpectra());
    const auto spec = (flatten) ? flsp : collapseIMSFrame(fpsp, clustMethodFromStr(method), mzWindow, minAbundance);
    
    return Rcpp::DataFrame::create(Rcpp::Named("ID") = spec.IDs,
                                   Rcpp::Named("mz") = spec.mzs,
                                   Rcpp::Named("intensity") = spec.intensities,
                                   Rcpp::Named("mobility") = spec.mobilities);
}

// [[Rcpp::export]]
Rcpp::List getTIMSPeakLists(const std::string &file, Rcpp::List frameIDsList,
                            const std::vector<double> &mobilityStarts,
                            const std::vector<double> &mobilityEnds, const std::string &method, double mzWindow,
                            unsigned minAbundance = 1, unsigned topMost = 0, unsigned minIntensityPre = 0,
                            unsigned minIntensityPost = 0, Rcpp::Nullable<Rcpp::List> scanStartsListN = R_NilValue,
                            Rcpp::Nullable<Rcpp::List> scanEndsListN = R_NilValue)
{
    const auto count = frameIDsList.size();
    TimsDataHandle TDH(file);
    std::vector<SpectrumIMS> peakLists(count);
    const auto clMethod = clustMethodFromStr(method);
    SpectrumFilterParams postFilter;
    postFilter.minIntensity = minIntensityPost;
    
    const bool subScans = scanStartsListN.isUsable() && scanEndsListN.isUsable();
    Rcpp::List scanStartsList, scanEndsList;
    if (subScans)
    {
        scanStartsList = scanStartsListN;
        scanEndsList = scanEndsListN;
    }
    
    for (int i=0; i<count; ++i)
    {
        const auto frameIDs = Rcpp::as<std::vector<unsigned>>(frameIDsList[i]);
        const SpectrumFilterParams preFilter(minIntensityPre, 0.0, 0.0, mobilityStarts[i], mobilityEnds[i]);
        
        if (!subScans)
            peakLists[i] = collapseIMSFrames(TDH, frameIDs, preFilter, postFilter, topMost, clMethod,
                                             mzWindow, minAbundance);
        else
        {
            Rcpp::List scanStartsItem = scanStartsList[i], scanEndsItem = scanEndsList[i];
            std::vector<std::vector<unsigned>> scanStarts, scanEnds;
            for (int j=0; j<scanStartsItem.size(); ++j)
            {
                scanStarts.push_back(Rcpp::as<std::vector<unsigned>>(scanStartsItem[j]));
                scanEnds.push_back(Rcpp::as<std::vector<unsigned>>(scanEndsItem[j]));
            }
            peakLists[i] = collapseIMSFrames(TDH, frameIDs, preFilter, postFilter, topMost, clMethod,
                                             mzWindow, minAbundance, scanStarts, scanEnds);
        }
    }

    Rcpp::List ret(peakLists.size());
    for (size_t i=0; i<peakLists.size(); ++i)
        ret[i] = Rcpp::DataFrame::create(Rcpp::Named("ID") = peakLists[i].IDs,
                                         Rcpp::Named("mz") = peakLists[i].mzs,
                                         Rcpp::Named("intensity") = peakLists[i].intensities,
                                         Rcpp::Named("mobility") = peakLists[i].mobilities);
    return ret;
}

// [[Rcpp::export]]
Rcpp::List getTIMSEIC(const std::string &file, const std::vector<unsigned> &frameIDs,
                      const std::vector<double> &mzStarts, const std::vector<double> &mzEnds,
                      const std::vector<double> &mobilityStarts, const std::vector<double> &mobilityEnds)
{
    TimsDataHandle TDH(file);
    std::vector<EIX> EICs(mzStarts.size());
    ThreadExceptionHandler exHandler;
    
    // UNDONE: make num_threads configurable
    #pragma omp parallel num_threads(8)
    {
        auto TBuffers = getTIMSDecompBuffers(TDH.get_decomp_buffer_size());
        std::vector<EIX> threadEICs(mzStarts.size());
        
        #pragma omp for nowait
        for (size_t i=0; i<frameIDs.size(); ++i)
        {
            exHandler.run([&]
            {
                auto &fr = TDH.get_frame(frameIDs[i]);
                if (fr.msms_type != 0)
                    return; // UNDONE: needed?
                const IMSFrame frame = getIMSFrame(fr, TBuffers, SpectrumFilterParams(), SpectrumScanRanges(), 0, -1.0,
                                                   false, false);
                const auto &mobs = frame.getMobilities();

                for (size_t j = 0; j < EICs.size(); ++j)
                {
                    unsigned inten = 0;
                    for (size_t k = 0; k < frame.size(); ++k)
                    {
                        // Rcpp::Rcout << "mob: " << mobs[k] << "/" << mobilityStarts[j] << "/" << mobilityEnds[j] << "\n";
                        if (mobilityStarts[j] != 0 && mobs[k] < mobilityStarts[j])
                            continue;
                        if (mobilityEnds[j] != 0 && mobs[k] > mobilityEnds[j])
                            continue;

                        const auto &sp = frame[k];
                        for (size_t l = 0; l < sp.size(); ++l)
                        {
                            if (numberGTE(sp.mzs[l], mzStarts[j], 10E-8) && numberLTE(sp.mzs[l], mzEnds[j], 10E-8))
                                inten += sp.intensities[l];
                        }
                    }
                    threadEICs[j].first.push_back(fr.time);
                    threadEICs[j].second.push_back(inten);
                }
            });
        }
        
        #pragma omp critical
        {
            for (size_t i=0; i<EICs.size(); ++i)
            {
                EICs[i].first.insert(EICs[i].first.end(), threadEICs[i].first.begin(), threadEICs[i].first.end());
                EICs[i].second.insert(EICs[i].second.end(), threadEICs[i].second.begin(), threadEICs[i].second.end());
            }
        }
    }
    exHandler.reThrow();
    
    Rcpp::List ret(EICs.size());
    for (size_t i=0; i<EICs.size(); ++i)
    {
        EIX eic = sortCompressEIX(EICs[i]);
        ret[i] = Rcpp::DataFrame::create(Rcpp::Named("time") = eic.first,
                                         Rcpp::Named("intensity") = eic.second);
    }
    return ret;
}

// [[Rcpp::export]]
Rcpp::List getTIMSMobilogram(const std::string &file, Rcpp::List frameIDsList, const std::vector<double> &mzStarts,
                             const std::vector<double> &mzEnds, const std::string &method, double IMSWindow,
                             unsigned topMost = 0, unsigned minIntensity = 0)
{
    const auto count = frameIDsList.size();
    TimsDataHandle TDH(file);
    const auto clMethod = clustMethodFromStr(method);
    std::vector<EIX> mobilograms(count);
    
    for (int i=0; i<count; ++i)
    {
        const auto frameIDs = Rcpp::as<std::vector<unsigned>>(frameIDsList[i]);
        const SpectrumFilterParams filterP(minIntensity, mzStarts[i], mzEnds[i], 0.0, 0.0);
        const auto frames = getTIMSFrames(TDH, frameIDs, filterP, topMost, -1.0, false, false);

        EIX flatEIM;
        for (size_t j = 0; j < frames.size(); ++j)
        {
            flatEIM.first.insert(flatEIM.first.end(), frames[j].getMobilities().begin(),
                                 frames[j].getMobilities().end());
            for (const auto &sp : frames[j].getSpectra())
                flatEIM.second.push_back(std::accumulate(sp.intensities.begin(), sp.intensities.end(), 0));
        }

        const auto clusts = clusterNums(flatEIM.first, clMethod, IMSWindow);
        const int maxClust = *(std::max_element(clusts.begin(), clusts.end()));
        EIX EIM = std::make_pair(std::vector<double>(maxClust + 1), std::vector<unsigned>(maxClust + 1));
        std::vector<unsigned> clSizes(maxClust + 1);

        // sum data, averaging is done below
        // NOTE: for now don't consider weighting averages, since there are likely to be clusters with zero intensities
        for (size_t i=0; i<clusts.size(); ++i)
        {
            const size_t cl = clusts[i];
            EIM.first[cl] += flatEIM.first[i];
            EIM.second[cl] += flatEIM.second[i];
            ++clSizes[cl];
        }

        // average data
        for (size_t i=0; i<EIM.first.size(); ++i)
        {
            EIM.first[i] /= static_cast<double>(clSizes[i]); // mean of all values
            EIM.second[i] /= frameIDs.size(); // mean of values (including frames without this cluster)
        }

        mobilograms[i] = sortCompressEIX(EIM);
    }
    
    Rcpp::List ret(mobilograms.size());
    for (size_t i=0; i<mobilograms.size(); ++i)
        ret[i] = Rcpp::DataFrame::create(Rcpp::Named("mobility") = mobilograms[i].first,
                                         Rcpp::Named("intensity") = mobilograms[i].second);
    return ret;
}

// [[Rcpp::export]]
Rcpp::List collapseTIMSSpectra(const std::string &file, const std::vector<unsigned> &frameIDs, double mzStart,
                               double mzEnd, double mobilityStart, double mobilityEnd, const std::string &method,
                               double mzWindow, unsigned minAbundance, unsigned topMost, unsigned minIntensityPre,
                               unsigned minIntensityPost)
{
#if 0
    TimsDataHandle TDH(file);
    const auto clMethod = clustMethodFromStr(method);
    const SpectrumFilterParams filterPre(minIntensityPre, mzStart, mzEnd, mobilityStart, mobilityEnd);
    const SpectrumFilterParams filterPost(minIntensityPost, 0.0, 0.0, 0.0, 0.0);

    std::vector<SpectrumIMS> spectra(frameIDs.size());
    ThreadExceptionHandler exHandler;
    
    // UNDONE: make num_threads configurable
    #pragma omp parallel num_threads(8)
    {
        auto TBuffers = getTIMSDecompBuffers(TDH.get_decomp_buffer_size());
        #pragma omp for
        for (size_t i=0; i<frameIDs.size(); ++i)
        {
            exHandler.run([&]
            {
                auto &fr = TDH.get_frame(frameIDs[i]);
                SpectrumIMS spec = getIMSFrame(fr, TBuffers);
                spec = filterSpectrum(spec, filterPre);
                spec = spectrumTopMost(spec, topMost);
                spectra[i] = filterSpectrum(collapseIMSFrame(spec, clMethod, clusterDataType::MZ, mzWindow, minAbundance),
                                            filterPost);
            });
        }
    }
    exHandler.reThrow();
    
    Rcpp::List ret(spectra.size());
    const auto coln = Rcpp::CharacterVector::create("mz", "intensity");
    for (size_t i=0; i<spectra.size(); ++i)
    {
        Rcpp::NumericMatrix m(spectra[i].size(), 2);
        Rcpp::NumericVector mzs = Rcpp::wrap(spectra[i].mzs), ints = Rcpp::wrap(spectra[i].intensities);
        m(Rcpp::_, 0) = mzs; m(Rcpp::_, 1) = ints;
        Rcpp::colnames(m) = coln;
        ret[i] = m;
    }
    return ret;
#endif
}
