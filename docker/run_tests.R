# unfortunately vdiffr doesn't allow to specify the deps file name.
# file.rename("tests/figs/deps-docker.txt", "tests/figs/deps.txt")

options(patRoon.progress.opts = list(style = 1))

install.packages(c("devtools", "vdiffr"))

# return failure exit code when tests fail: https://github.com/r-lib/testthat/issues/515
tret <- as.data.frame(devtools::test(reporter = testthat::MultiReporter$new(list(testthat::SummaryReporter$new(),
                                                                                 testthat::JunitReporter$new(file = "~/junit.xml")))))
print(tret)
if (sum(tret$failed) > 0 || any(tret$error))
    q(status = 1)
