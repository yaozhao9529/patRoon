% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/components-clust.R
\docType{class}
\name{componentsClust-class}
\alias{componentsClust-class}
\alias{componentsClust}
\alias{delete,componentsClust-method}
\alias{clusters,componentsClust-method}
\alias{cutClusters,componentsClust-method}
\alias{clusterProperties,componentsClust-method}
\alias{treeCut,componentsClust-method}
\alias{treeCutDynamic,componentsClust-method}
\alias{plot,componentsClust,missing-method}
\alias{plotSilhouettes,componentsClust-method}
\title{Base class for components that are based on hierarchical clustered data.}
\usage{
\S4method{delete}{componentsClust}(obj, ...)

\S4method{clusters}{componentsClust}(obj)

\S4method{cutClusters}{componentsClust}(obj)

\S4method{clusterProperties}{componentsClust}(obj)

\S4method{treeCut}{componentsClust}(obj, k = NULL, h = NULL)

\S4method{treeCutDynamic}{componentsClust}(obj, maxTreeHeight, deepSplit, minModuleSize)

\S4method{plot}{componentsClust,missing}(
  x,
  pal = "Paired",
  numericLabels = TRUE,
  colourBranches = length(x) < 50,
  showLegend = length(x) < 20,
  ...
)

\S4method{plotSilhouettes}{componentsClust}(obj, kSeq, pch = 16, type = "b", ...)
}
\arguments{
\item{\dots}{Further options passed to \code{\link{plot.dendrogram}} (\code{plot}) or \code{\link[graphics]{plot}}
(\code{plotSilhouettes}).}

\item{k, h}{Desired number of clusters or tree height to be used for cutting the dendrogram, respectively. One or the
other must be specified. Analogous to \code{\link{cutree}}.}

\item{maxTreeHeight, deepSplit, minModuleSize}{Arguments used by
\code{\link{cutreeDynamicTree}}.}

\item{x, obj}{A \code{componentsClust} (derived) object.}

\item{pal}{Colour palette to be used from \pkg{\link{RColorBrewer}}.}

\item{numericLabels}{Set to \code{TRUE} to label with numeric indices instead of (long) feature group names.}

\item{colourBranches}{Whether branches from cut clusters (and their labels)
should be coloured. Might be slow with large numbers of clusters, hence,
the default is only \code{TRUE} when this is not the case.}

\item{showLegend}{If \code{TRUE} and \code{colourBranches} is also
\code{TRUE} then a legend will be shown which outlines cluster numbers and
their colours. By default \code{TRUE} for small amount of clusters to avoid
overflowing the plot.}

\item{kSeq}{An integer vector containing the sequence that should be used for
average silhouette width calculation.}

\item{pch, type}{Passed to \code{\link[graphics]{plot}}.}
}
\description{
This base class is derived from \code{\link{components}} and is used to store components resulting from hierarchical
clustering information, for instance, generated by \code{\link{generateComponentsIntClust}} and
\code{\link{generateComponentsSpecClust}}.
}
\section{Methods (by generic)}{
\itemize{
\item \code{clusters(componentsClust)}: Accessor method to the \code{clust} slot, which was generated by \code{\link{hclust}}.

\item \code{cutClusters(componentsClust)}: Accessor method to the \code{cutClusters} slot. Returns a vector with cluster membership
for each candidate (format as \code{\link{cutree}}).

\item \code{clusterProperties(componentsClust)}: Returns a list with properties on how the
clustering was performed.

\item \code{treeCut(componentsClust)}: Manually (re-)cut the dendrogram.

\item \code{treeCutDynamic(componentsClust)}: Automatically (re-)cut the dendrogram using the \code{\link{cutreeDynamicTree}} function
from \pkg{\link{dynamicTreeCut}}.

\item \code{plot(x = componentsClust, y = missing)}: generates a dendrogram from a given cluster object and optionally highlights resulting
branches when the cluster is cut.

\item \code{plotSilhouettes(componentsClust)}: Plots the average silhouette width when the
clusters are cut by a sequence of k numbers. The k value with the highest
value (marked in the plot) may be considered as the optimal number of
clusters.

}}
\section{Slots}{

\describe{
\item{\code{distm}}{Distance matrix that was used for clustering (obtained with \code{\link{daisy}}).}

\item{\code{clust}}{Object returned by \code{\link{hclust}}.}

\item{\code{cutClusters}}{A \code{list} with assigned clusters (same format as what \code{\link{cutree}} returns).}

\item{\code{gInfo}}{The \code{\link{groupInfo}} of the feature groups object that was used.}

\item{\code{properties}}{A list containing general properties and parameters used for clustering.}

\item{\code{altered}}{Set to \code{TRUE} if the object was altered (\emph{e.g.} filtered) after its creation.}
}}

\note{
The intensity values for components (used by \code{plotSpectrum}) are set
  to a dummy value (1) as no single intensity value exists for this kind of
  components.

When the object is altered (\emph{e.g.} by filtering or subsetting it), methods that need the original
  clustered data such as plotting methods do not work anymore and stop with an error.
}
\section{S4 class hierarchy}{
 \itemize{   \item{\code{\link{components}}}   \itemize{     \item{\strong{\code{\link{componentsClust}}}}     \itemize{       \item{\code{\link{componentsIntClust}}}       \item{\code{\link{componentsSpecClust}}}     }   } }
}

\references{
\insertRef{Scholle2018}{patRoon}
}
\seealso{
\code{\link{components}} and \code{\link{generateComponents}}
}
