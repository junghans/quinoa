
ADD_SUBDIRECTORY(Lesson01-Init)
ADD_SUBDIRECTORY(Lesson02-Map-Vector)
ADD_SUBDIRECTORY(Lesson03-Power-Method)

# mfh 08 Nov 2013: Lesson 04 doesn't currently require a build or test
# step.  If you add one or more examples to that directory that do
# require building and testing, please add that subdirectory here.

ADD_SUBDIRECTORY(Lesson05-Redistribution)
ADD_SUBDIRECTORY(Lesson06-Custom-Operator)
ADD_SUBDIRECTORY(Lesson07-Kokkos-Fill)

ADD_SUBDIRECTORY(advanced)
