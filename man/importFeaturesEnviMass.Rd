% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/features-envipick.R
\name{importFeaturesEnviMass}
\alias{importFeaturesEnviMass}
\title{Imports features from enviMass}
\usage{
importFeaturesEnviMass(analysisInfo, enviProjPath)
}
\arguments{
\item{analysisInfo}{A \code{data.frame} with \link[=analysis-information]{Analysis information}.}

\item{enviProjPath}{The path of the enviMass project.}
}
\value{
An object of a class which is derived from \code{\link{features}}.
}
\description{
Imports features from a project generated by the \pkg{enviMass} package.
}
\details{
This function imports data from enviMass. This function is called when calling \code{importFeatures} with
  \code{type="envimass"}.
}
\note{
This functionality has only been tested with older versions of \pkg{enviMass}.
}
\seealso{
\code{\link{importFeatures}} for more details and other algorithms.
}
