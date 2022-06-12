#include <cstdint>
#include <vector>

#include <omp.h>

#include "opentims++/opentims_all.h"

#include <Rcpp.h>

#include "utils.h"

namespace {

struct SpectrumIMS // UNDONE: merge with other struct(s)
{
    std::vector<unsigned> IDs, intensities;
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
    void addData(const SpectrumIMS &sp)
    {
        IDs.insert(IDs.end(), sp.IDs.begin(), sp.IDs.end());
        mzs.insert(mzs.end(), sp.mzs.begin(), sp.mzs.end());
        intensities.insert(intensities.end(), sp.intensities.begin(), sp.intensities.end());
        mobilities.insert(mobilities.end(), sp.mobilities.begin(), sp.mobilities.end());
    }
    void resize(size_t s)
    {
        IDs.resize(s);
        mzs.resize(s);
        intensities.resize(s);
        mobilities.resize(s);
    }
    size_t size(void) const { return IDs.size(); }
};

    
SpectrumIMS getIMSFrame(TimsFrame &frame)
{
    SpectrumIMS spec(frame.num_peaks);
    frame.save_to_buffs(nullptr, spec.IDs.data(), nullptr, spec.intensities.data(), spec.mzs.data(),
                        spec.mobilities.data(), nullptr);
    return spec;
}

SpectrumIMS filterSpectrum(const SpectrumIMS &spec, unsigned scanBegin = 0, unsigned scanEnd = 0,
                           double mzStart = 0.0, double mzEnd = 0.0, double mobilityStart = 0.0,
                           double mobilityEnd = 0.0)
{
    if (scanBegin == 0 && scanEnd == 0 && mzStart == 0.0 && mzEnd == 0.0 && mobilityStart == 0.0 &&
        mobilityEnd == 0.0)
        return spec;
    
    SpectrumIMS specFiltered;
    for (size_t i=0; i<spec.size(); ++i)
    {
        if (scanBegin != 0 && spec.IDs[i] < scanBegin)
            continue;
        if (scanEnd != 0 && spec.IDs[i] > scanEnd)
            continue;
        if (mzStart != 0.0 && spec.mzs[i] < mzStart)
            continue;
        if (mzEnd != 0.0 && spec.mzs[i] > mzEnd)
            continue;
        if (mobilityStart != 0.0 && spec.mobilities[i] < mobilityStart)
            continue;
        if (mobilityEnd != 0.0 && spec.mobilities[i] > mobilityEnd)
            continue;
        
        specFiltered.addData(spec.IDs[i], spec.mzs[i], spec.intensities[i], spec.mobilities[i]);
    }
    return specFiltered;
}

SpectrumIMS collapseIMSFrame(const SpectrumIMS &frame, clusterMethod method, double mzWindow)
{
    const std::vector<int> clusts = clusterNums(frame.mzs, method, mzWindow);
    const int maxClust = *(std::max_element(clusts.begin(), clusts.end()));
    SpectrumIMS binnedSpectrum(maxClust + 1);
    std::vector<unsigned> binSizes(maxClust + 1);
    
    // assign unique IDs
    std::iota(binnedSpectrum.IDs.begin(), binnedSpectrum.IDs.end(), 1);
    
    // sum data for each cluster
    for (size_t i=0; i<clusts.size(); ++i)
    {
        const size_t cl = clusts[i];
        binnedSpectrum.mzs[cl] += frame.mzs[i];
        binnedSpectrum.intensities[cl] += frame.intensities[i];
        binnedSpectrum.mobilities[cl] += frame.mobilities[i];
        ++binSizes[cl];
    }

    // average data
    for (size_t i=0; i<binnedSpectrum.size(); ++i)
    {
        const double len = static_cast<double>(binSizes[i]);
        binnedSpectrum.mzs[i] /= len;
        binnedSpectrum.mobilities[i] /= len;
    }

    return binnedSpectrum;
}

SpectrumIMS collapseIMSFrames(TimsDataHandle &TDH, const std::vector<unsigned> &frameIDs, clusterMethod method,
                              double mzWindow)
{
    SpectrumIMS ret;
    int specN = 0;
    
    for (auto i : frameIDs)
    {
        if (!TDH.has_frame(i))
            continue;
        auto &fr = TDH.get_frame(i);
        const SpectrumIMS spec = getIMSFrame(fr); // UNDONE: filter args
        ret.addData(collapseIMSFrame(spec, method, mzWindow));
        ++specN;
    }
    
    // collapse result
    ret = collapseIMSFrame(ret, method, mzWindow);
    // average intensities
    for (auto &inten : ret.intensities)
        inten /= specN;
    
    return ret;
}

}

// [[Rcpp::export]]
void initBrukerLibrary(const std::string &path)
{
    setup_bruker(path);
}

// [[Rcpp::export]]
Rcpp::DataFrame collapseTIMSFrame(const std::string &file, size_t frameID, const std::string &method, double mzWindow)
{
    TimsDataHandle TDH(file);
    if (!TDH.has_frame(frameID))
        Rcpp::stop("Frame doesn't exist.");
    
    const SpectrumIMS frame = getIMSFrame(TDH.get_frame(frameID));
    const auto spec = collapseIMSFrame(frame, clustMethodFromStr(method), mzWindow);
    
    return Rcpp::DataFrame::create(Rcpp::Named("ID") = spec.IDs,
                                   Rcpp::Named("mz") = spec.mzs,
                                   Rcpp::Named("intensity") = spec.intensities,
                                   Rcpp::Named("mobility") = spec.mobilities);
}

// [[Rcpp::export]]
Rcpp::DataFrame collapseTIMSFrames(const std::string &file, const std::vector<unsigned> &frameIDs,
                                   const std::string &method, double mzWindow)
{
    TimsDataHandle TDH(file);
    const auto spec = collapseIMSFrames(TDH, frameIDs, clustMethodFromStr(method), mzWindow);
    return Rcpp::DataFrame::create(Rcpp::Named("ID") = spec.IDs,
                                   Rcpp::Named("mz") = spec.mzs,
                                   Rcpp::Named("intensity") = spec.intensities,
                                   Rcpp::Named("mobility") = spec.mobilities);
}

// [[Rcpp::export]]
Rcpp::List getTIMSEIC(const std::string &file, const std::vector<unsigned> &frameIDs,
                      std::vector<double> mzStarts, std::vector<double> mzEnds, std::vector<double> mobilityStarts,
                      std::vector<double> mobilityEnds)
{
    TimsDataHandle TDH(file);
    struct EIC // UNDONE?
    {
        std::vector<double> times, intensities;
    };
    std::vector<EIC> EICs(mzStarts.size());
    // ThreadingManager::get_instance().set_num_threads(1);
    
#if 1
    
    #pragma omp parallel num_threads(1)
    {
        // get buffers for decompression (see TimsDataHandle::extract_frames()) 
        std::unique_ptr<ZSTD_DCtx, decltype(&ZSTD_freeDCtx)> zstd(ZSTD_createDCtx(), &ZSTD_freeDCtx);
        std::unique_ptr<char[]> decompBuffer = std::make_unique<char[]>(TDH.get_decomp_buffer_size());
        std::vector<EIC> threadEICs(mzStarts.size());
        
        //Rcpp::Rcout << "thr: " << omp_get_thread_num() << "/" << omp_get_num_threads() << "\n";
        
        #pragma omp for nowait
        for (size_t i=0; i<frameIDs.size(); ++i)
        {
            auto &fr = TDH.get_frame(frameIDs[i]);
            if (fr.msms_type != 0)
                continue; // UNDONE?
            fr.decompress(decompBuffer.get(), zstd.get());
            const SpectrumIMS spec = getIMSFrame(fr);
            fr.close();
            
            for (size_t j=0; j<EICs.size(); ++j)
            {
                const SpectrumIMS frameF = filterSpectrum(spec, 0, 0, mzStarts[j], mzEnds[j], mobilityStarts[j],
                                                          mobilityEnds[j]);
                threadEICs[j].times.push_back(fr.time);
                threadEICs[j].intensities.push_back(std::accumulate(frameF.intensities.begin(), frameF.intensities.end(), 0));
                // UNDONE: clear zeros intensities if neighbours are? And always add times to avoid gaps?
            }
        }
        
        #pragma omp critical
        {
            for (size_t i=0; i<EICs.size(); ++i)
            {
                EICs[i].times.insert(EICs[i].times.end(), std::make_move_iterator(threadEICs[i].times.begin()),
                                     std::make_move_iterator(threadEICs[i].times.end()));
                EICs[i].intensities.insert(EICs[i].intensities.end(),
                                           std::make_move_iterator(threadEICs[i].intensities.begin()),
                                           std::make_move_iterator(threadEICs[i].intensities.end()));
            }
        }
    }
#else
    const auto maxPeaks = TDH.max_peaks_in_frame();
    // NOTE: tofs not used, but will otherwise be allocated in save_to_buffs()
    std::vector<uint32_t> IDs(maxPeaks), intensities(maxPeaks), tofs(maxPeaks);
    std::vector<double> mzs(maxPeaks), mobilities(maxPeaks);
    
    for (auto i : frameIDs)
    {
        auto &fr = TDH.get_frame(i);
        if (fr.msms_type != 0)
            continue; // UNDONE?
        
        fr.save_to_buffs(nullptr, IDs.data(), tofs.data(), intensities.data(), mzs.data(),
                         mobilities.data(), nullptr);
        
        for (size_t j=0; j<EICs.size(); ++j)
        {
            EICs[j].times.push_back(fr.time);
            EICs[j].intensities.push_back(0);
            for (size_t k=0; k<fr.num_peaks; ++k)
            {
                if (mzStarts[j] != 0.0 && mzs[k] < mzStarts[j])
                    continue;
                if (mzEnds[j] != 0.0 && mzs[k] > mzEnds[j])
                    continue;
                if (mobilityStarts[j] != 0.0 && mobilities[k] < mobilityStarts[j])
                    continue;
                if (mobilityEnds[j] != 0.0 && mobilities[k] > mobilityEnds[j])
                    continue;
                
                EICs[j].intensities.back() += intensities[k];
            }
        }
    }
#endif
    Rcpp::List ret(EICs.size());
    for (size_t i=0; i<EICs.size(); ++i)
        ret[i] = Rcpp::DataFrame::create(Rcpp::Named("time") = EICs[i].times,
                                         Rcpp::Named("intensity") = EICs[i].intensities);
    return ret;
}
