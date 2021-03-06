# See cmake/add_regression_test.cmake for documentation on the arguments to
# add_regression_test().

if (HAS_MKL)
  add_regression_test(OrnsteinUhlenbeck_mkl ${WALKER_EXECUTABLE}
                      NUMPES 1
                      INPUTFILES ou_mkl.q stat.txt.std ou.ndiff.cfg
                      ARGS -c ou_mkl.q -v
                      TEXT_BASELINE stat.txt.std
                      TEXT_RESULT stat.txt
                      TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

  add_regression_test(OrnsteinUhlenbeck_mkl ${WALKER_EXECUTABLE}
                      NUMPES 2
                      INPUTFILES ou_mkl.q stat.txt.std ou.ndiff.cfg
                      ARGS -c ou_mkl.q -v
                      TEXT_BASELINE stat.txt.std
                      TEXT_RESULT stat.txt
                      TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

  add_regression_test(OrnsteinUhlenbeck_mkl ${WALKER_EXECUTABLE}
                      NUMPES 4
                      INPUTFILES ou_mkl.q stat.txt.std ou.ndiff.cfg
                      ARGS -c ou_mkl.q -v
                      TEXT_BASELINE stat.txt.std
                      TEXT_RESULT stat.txt
                      TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

  add_regression_test(OrnsteinUhlenbeck_mkl_u0.9 ${WALKER_EXECUTABLE}
                      NUMPES 4
                      INPUTFILES ou_mkl.q stat.txt.std ou.ndiff.cfg
                      ARGS -c ou_mkl.q -v -u 0.9
                      TEXT_BASELINE stat.txt.std
                      TEXT_RESULT stat.txt
                      TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

  add_regression_test(OrnsteinUhlenbeck_mkl_u0.9 ${WALKER_EXECUTABLE}
                      NUMPES 8
                      INPUTFILES ou_mkl.q stat.txt.std ou.ndiff.cfg
                      ARGS -c ou_mkl.q -v -u 0.9
                      TEXT_BASELINE stat.txt.std
                      TEXT_RESULT stat.txt
                      TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

  add_regression_test(OrnsteinUhlenbeckPDF_mkl ${WALKER_EXECUTABLE}
                      NUMPES 8
                      INPUTFILES ou_pdf_mkl.q pdf_f1.txt.std ou_pdf.ndiff.cfg
                      ARGS -c ou_pdf_mkl.q -v
                      LABELS verification
                      TEXT_BASELINE pdf_f1.txt.std
                      TEXT_RESULT pdf_f1.txt
                      TEXT_DIFF_PROG_CONF ou_pdf.ndiff.cfg)
endif()

add_regression_test(OrnsteinUhlenbeck ${WALKER_EXECUTABLE}
                    NUMPES 1
                    INPUTFILES ou.q stat.txt.std ou.ndiff.cfg
                    ARGS -c ou.q -v
                    TEXT_BASELINE stat.txt.std
                    TEXT_RESULT stat.txt
                    TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

add_regression_test(OrnsteinUhlenbeck ${WALKER_EXECUTABLE}
                    NUMPES 2
                    INPUTFILES ou.q stat.txt.std ou.ndiff.cfg
                    ARGS -c ou.q -v
                    TEXT_BASELINE stat.txt.std
                    TEXT_RESULT stat.txt
                    TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

add_regression_test(OrnsteinUhlenbeck ${WALKER_EXECUTABLE}
                    NUMPES 4
                    INPUTFILES ou.q stat.txt.std ou.ndiff.cfg
                    ARGS -c ou.q -v
                    TEXT_BASELINE stat.txt.std
                    TEXT_RESULT stat.txt
                    TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

add_regression_test(OrnsteinUhlenbeck_u0.9 ${WALKER_EXECUTABLE}
                    NUMPES 4
                    INPUTFILES ou.q stat.txt.std ou.ndiff.cfg
                    ARGS -c ou.q -v -u 0.9
                    TEXT_BASELINE stat.txt.std
                    TEXT_RESULT stat.txt
                    TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

add_regression_test(OrnsteinUhlenbeck_u0.9 ${WALKER_EXECUTABLE}
                    NUMPES 8
                    INPUTFILES ou.q stat.txt.std ou.ndiff.cfg
                    ARGS -c ou.q -v -u 0.9
                    TEXT_BASELINE stat.txt.std
                    TEXT_RESULT stat.txt
                    TEXT_DIFF_PROG_CONF ou.ndiff.cfg)

add_regression_test(OrnsteinUhlenbeckPDF ${WALKER_EXECUTABLE}
                    NUMPES 8
                    INPUTFILES ou_pdf.q pdf_f1.txt.std ou_pdf.ndiff.cfg
                    ARGS -c ou_pdf.q -v
                    LABELS verification
                    TEXT_BASELINE pdf_f1.txt.std
                    TEXT_RESULT pdf_f1.txt
                    TEXT_DIFF_PROG_CONF ou_pdf.ndiff.cfg)
