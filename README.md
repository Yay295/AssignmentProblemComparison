## What is this?

This repository is for comparing various implementations of solvers of the [Assignment Problem](https://en.wikipedia.org/wiki/Assignment_problem). I have tried to find every implementation I can, written in any programming language.

## Methodology

Before comparing their performance, I tested whether each implementation was even correct. I did not do an exhaustive test, rather, I checked each of the implementations against a few matrices that I had had problems with while writing my own implementation. If you find any more matrices I should test with, [create an issue for it](https://github.com/Yay295/AssignmentProblemComparison/issues/new). The three matrices being used for this test are:
```
0 0
0 0
```
```
0 0 0 0
0 0 0 0
0 0 0 0
0 0 0 1
```
```
0 0 0 0 0
0 0 0 0 0
0 0 1 1 1
0 0 1 1 1
0 0 1 1 1
```
While fairly simple, the fact that not all of the implementations pass this test shows their use.

Then I ran a speed test (on the implementations that passed the first test), timing how long it took them to solve a certain number of random matrices of a given size. Due to differences in programming language and timing, these will not be exact, but I have tried to be as accurate as possible by timing the least amount of code possible (i.e. not timing the initialization of matrices).

Some of the implementations had to be modified for testing. I have uploaded their original state here first, and then the changes I made in a different commit so that you can see what was changed.

## Results

I have entered these times as relative to my own code (< 1.0 is slower, > 1.0 is faster). The actual times I recorded and the calculation I used for this value are listed at the end. The Version Used is the date that the code I tested was last updated. Some of these have been tested more recently, but their code has not changed since the date listed. If any of these are out of date, [create an issue for it](https://github.com/Yay295/AssignmentProblemComparison/issues/new) and I'll see about re-testing it.

| Author                    | Language   | Version Used  | Test 1 | Test 2 | Test 3 | Speed  |
|---------------------------|------------|---------------|--------|--------|--------|--------|
| [Ben Chaplin]             | Python     | May  24, 2020 | Pass*  | Pass*  | Fail   |        |
| [Brian M. Clapper]        | Python     | Feb. 15, 2019 | Pass   | Pass   | Pass   |   0.01 |
| [dlib]                    | C++        | Mar. 28, 2021 | Pass   | Fail   | Fail   |        |
| [HungarianAlgorithm.com]  | ?          | Jan. 20, 2022 | Pass   | Pass   | Pass   |        |
| [John Weaver]             | C++        | Mar. 27, 2016 | Pass   | Pass   | Pass   |   0.68 |
| [Kevin L. Stern]          | Java       | May   7, 2017 | Pass   | Pass   | Pass   | 108.18 |
| [Mattias Andrée]          | C          | Feb. 20, 2021 | Pass   | Pass   | Pass   |   1.09 |
| [Paolo Bonzini]           | C          | June  9, 2017 | Pass   | Pass   | Pass   |   0.62 |
| [University of Melbourne] | JavaScript | Jan. 20, 2022 | Pass   | Pass   | Pass   |        |
| [Vamsi Kundeti]           | C++        | Dec. 20, 2008 | ?      | Fail   | Fail   |        |
| [Yay295]                  | C++        | Apr. 21, 2019 | Pass   | Pass   | Pass   |   1.00 |

### Notes

[Ben Chaplin]'s code has a bug where it doesn't work if any input value is 0. In order to at least *try* testing it the test case values were all incremented by 1. It still gets stuck in an infinite loop somewhere during the third test.

[dlib]'s code is not included in this repository because it's a pretty big library, and I did not modify it at all. The version I tested was 19.22, downloaded on January 20, 2022. I used the file modification date in the table above since I don't know exactly when the files I used were last changed.

Although the [HungarianAlgorithm.com] implementation passed my tests, I did not time it because it is a webpage, and the network transfer times would have dwarfed any actual calculation times. Also, it only allows matrices up to 10x10.

[John Weaver]'s code was originally written without templates, and then poorly ported to use templates. It had to be modified in order to correct those mistakes so that it could compile.

[Kevin L. Stern]'s code is incredibly fast, and I don't know why. I've noticed in the task manager that it is using multiple threads, but there's no multi-threading code in it, so this is probably just the garbage collector. It also seems to be caching more values, leading to a significantly higher memory footprint (though the memory usage could also just be the JVM).

[Paolo Bonzini]'s code is an updated fork of [Mattias Andrée]'s code. Considering it's slower and emits more warnings, I'm not sure it was a worthwhile update.

The [University of Melbourne] code doesn't actually solve it for you, but guides you along to a solution. I included it anyways since it must have some code to be able to guide you "correctly".

[Vamsi Kundeti]'s code passed my first test without crashing, but I wasn't able to determine what result it found. The code is a mess, and since it crashed on the second and third test I didn't bother checking the first more closely. I have not included the code in this repository because it is a somewhat large library. I *have* included the Python script I used to generate the test cases. This library takes a file as input, with the matrix to check formatted in a specific way. My Python script generates files in the correct format to be used by this library.

### Actual Times

These times are all averages, displayed in seconds. The same code was used to test the C and C++ implementations.

| Author             | Language | Compiler        | 10,000 50x50 | 100 250x250 | 10 1000x1000 |
|--------------------|----------|-----------------|--------------|-------------|--------------|
| [Brian M. Clapper] | Python   | 3.8.10          | 0.0875022136 | 15.24417355 |    N/A       |
| [John Weaver]      | C++      | gcc 9.3.0       | 0.000535937  |  0.142813   |   28.6266    |
| [Kevin L. Stern]   | Java     | OpenJDK 11.0.13 | 0.0000660661 |  0.00190706 |    0.0476911 |
| [Mattias Andrée]   | C        | gcc 9.3.0       | 0.000600     |  0.097656   |    8.685937  |
| [Paolo Bonzini]    | C        | gcc 9.3.0       | 0.001112     |  0.165313   |   15.579688  |
| [Yay295]           | C++      | gcc 9.3.0       | 0.000479688  |  0.100469   |   12.6188    |

### Speed Calculation

The equation I used for the "Speed" value is:

```python
my_test_1 = 0.000479688
my_test_2 = 0.100469
my_test_3 = 12.6188
other_test_1 = 0.000600
other_test_2 = 0.097656
other_test_3 = 8.685937
speed = ((my_test_1/other_test_1) + (my_test_2/other_test_2) + (my_test_3/other_test_3)) / 3
```

[Ben Chaplin]: https://github.com/benchaplin/hungarian-algorithm
[Brian M. Clapper]: https://github.com/bmc/munkres
[dlib]: http://dlib.net/
[HungarianAlgorithm.com]: http://hungarianalgorithm.com/solve.php
[John Weaver]: https://github.com/saebyn/munkres-cpp
[Kevin L. Stern]: https://github.com/KevinStern/software-and-algorithms/blob/master/src/main/java/blogspot/software_and_algorithms/stern_library/optimization/HungarianAlgorithm.java
[Lantao Liu]: http://robotics.usc.edu/~lantao/codes/hungarian.php
[Mattias Andrée]: https://github.com/maandree/hungarian-algorithm-n3
[Paolo Bonzini]: https://github.com/bonzini/hungarian-algorithm-n3
[University of Melbourne]: http://www.ifors.ms.unimelb.edu.au/tutorial/hungarian/welcome_frame.html
[Vamsi Kundeti]: https://launchpad.net/lib-bipartite-match
[Yay295]: https://github.com/Yay295/Assignment-Problem-Solver
