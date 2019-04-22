# under construction

The times at the bottom are old (though most of them shouldn't change much). Once I have all of the implementations updated I will rerun all of the tests.

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

I have entered these times as relative to my own code (< 1.0 is slower, > 1.0 is faster). The actual times I recorded are listed at the end. The Version Used is the date that the code I tested was last updated. Some of these have been tested more recently, but their code has not changed since the date listed. If any of these are out of date, [create an issue for it](https://github.com/Yay295/AssignmentProblemComparison/issues/new) and I'll see about re-testing it.

| Author                    | Language   | Version Used  | Test 1 | Test 2 | Test 3 | Speed |
|---------------------------|------------|---------------|--------|--------|--------|-------|
| [Brian M. Clapper]        | Python     | Feb. 15, 2019 | Pass   | Pass   | Pass   |       |
| [dlib]                    | C++        | Mar. 10, 2019 | Pass   | Pass   | Fail   |       |
| [HungarianAlgorithm.com]  | ?          | Apr. 21, 2019 | Pass   | Pass   | Pass   |       |
| [John Weaver]             | C++        | Mar. 27, 2016 | Pass   | Pass   | Pass   |       |
| [Kevin L. Stern]          | Java       | Nov. 19, 2016 | Pass   | Pass   | Pass   |       |
| Lantao Liu                | C++        | June 18, 2013 | Pass   | Fail   | Fail   |       |
| [Mattias Andrée]          | C          | Apr. 24, 2014 | Pass   | Pass   | Pass   |       |
| [Paolo Bonzini]           | C          | June  9, 2017 | Pass   | Pass   | Pass   |       |
| [University of Melbourne] | JavaScript | Apr. 21, 2019 | Pass   | Pass   | Fail   |       |
| [Vamsi Kundeti]           | C++        | Dec. 20, 2008 | ?      | Fail   | Fail   |       |
| [Yay295]                  | C++        | Apr. 21, 2019 | Pass   | Pass   | Pass   |       |

### Notes

Although the [HungarianAlgorithm.com] implementation passed my tests, I did not time it because it is a webpage, and the network transfer times would have dwarfed any actual calculation times. Also, it only allows matrices up to 10x10.

[John Weaver]'s code was originally written without templates, and then poorly ported to use templates. It had to be modified in order to correct those mistakes so that it could compile.

[Kevin L. Stern]'s code is incredibly fast, and I don't know why. I've already spent longer on this than I planned though, so I'm just going to present them as they are and maybe come back to this in the future.

Lantao Liu's code didn't compile until making modifications to `Matrix.h`. Its original source was http://robotics.usc.edu/~lantao/codes/hungarian.php, but I can no longer find it online. Lantao Liu has a website at http://iurobotics.net/lantaoliu.html.

[Paolo Bonzini]'s code is an updated fork of [Mattias Andrée]'s code. Considering it's slower and emits more warnings, I'm not sure it was a worthwhile update.

The [University of Melbourne] code doesn't actually solve it for you, but guides you along to a solution. I included it anyways since it must have some code to be able to guide you "correctly".

[Vamsi Kundeti]'s code passed my first test without crashing, but I wasn't able to determine what result it found. The code is a mess, and since it crashed on the second and third test I didn't bother checking the first more closely.

### Actual Times

These times are all averages, displayed in seconds. The same code was used to test the C and C++ implementations.

| Author                   | Language | 10,000 50x50 | 100 250x250 | 10 1000x1000 |
|--------------------------|----------|--------------|-------------|--------------|
| [Brian M. Clapper]       | Python   | 0.0873396668 | 15.3306751  | 1277.492758  |
| [John Weaver]            | C++      | 0.0007233    |  0.23719    |   61.9894    |
| [Kevin L. Stern]         | Java     | 0.000093     |  0.002968   |    0.072743  |
| [Mattias Andrée]         | C        | 0.001264     |  0.21       |   17.2625    |
| [Paolo Bonzini]          | C        | 0.002141     |  0.349062   |   31.679688  |
| [Yay295]                 | C++      | 0.0006636    |  0.13706    |   19.5977    |

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
