/*!

\file self_organizing.h
\brief The only file you need.
\author Thierry Seegers

\mainpage self_organizing

\tableofcontents

\section introduction Introduction

\subsection why Why this project?

I started this project the same way I started a few others already.
I cracked open my "Data Structures and Algorithm Analysis"<SUP>[1]</SUP> book from college and browsed its table of contents for an interesting concept to implement in C++11.
This project is about exploring what the new C++ standard offers and putting it in practice in smallish, self-contained libraries.
I'm also using this project to try out GitHub's offering.
This project is \e not the most perfomant and efficient self-orginizing list you've ever seen.
In fact, after running some crude performance benchmark, I've concluded its advantage window is quite narrow.
More on that down below.

\subsection whatis What is a self-organizing list?

A self-organizing list is a container that attempts to optimize future searches based on past searches.
Because a search in a list is a linear operation, the closer to the head an element is, the faster it is to find it.
Popular elements should therefore move closer to the head of the list over time to optimize future searches.
Less popular elements will drift to the tail of the list.
You might recognize in this description an implementation of the "80/20" rule for lists.

Different self-organizing strategies exist, the following three are offered in this library.
\li \b Count. Elements are kept ordered by the frequency of past searches.
 An element that has been searched for \c N times is moved ahead of all elements that have been searched for less than \c N times.
\li \b Transpose. When an element is searched for, it is swapped with the element in front of it.
\li \b Move-to-front. When an element is searched for, it is moved to the front of the list.

\section considerations Technical considerations

Because of the motivation for this project to learn C++11 and put it in practice, you will obviously need a C++11 compliant compiler and library.
I'm using Visual Studio 2012 to develop but I additionally test with g++ 4.6 and XCode 4.5.
I'm not using advanced or arcane C++11 features, so earlier version of these toolchains might also work.

\section principles Design principles and considerations

\subsection policies Policy-based design

The public API of this library consists of two container classes and three policy classes to be used as template parameters for the container classes.

The two container classes are \ref self_organizing::list and \ref self_organizing::vector.
They distinguish themselves by which container from the \c std namespace they use to hold data.
Why offer \c std::vector as a possible data holder for something called "self-organizing list"?
Because I can.
But also because it helps to compare performance of such an animal with a that of a self-organizing list implemented with an actual list.

The policy classes each implement a self-organizing strategy.
All three classes are defined in \ref self_organizing::find_policy.

\subsection search Searching for an element

Searching for an element in a \c std::vector or a \c std::list is performed with the generic \c std::find from the <tt>&lt;algorithm&gt;</tt> header.
On the ther hand, \ref self_organizing::list and \ref self_organizing::vector provide \ref self_organizing::detail::container::find "find" member function.
It is important to note that, by definition, a container's elements are re-ordered when a \c find operation is performed.
Therefore, the \c find member is not \c const.

\subsection iterators Iterator invalidation

Iterator invalidation rules for a self-organizing container follow the same rules as for containers from the \c std namespace with one exception.
\ref self_organizing::vector::iterator is invalidated after performing a search.
This follows from the facts that a self-organizing container is re-ordered after a search is performed and that when elements from a \c std::vector are re-ordered, it's iterators are invalidated.
This exception does not apply to \ref self_organizing::list::iterator.

\section performance Performance

So, are they worth it?

We already know that when searching for elements in a container in a purely random fashion, a self-organizing list will offer no performance improvement, only overhead.
There would indeed be no useful information from past searches to use and optimize future searches.
Self-organizing lists must only be used when searches are not uniformly random.

I'll state upfront that if one considers only the time taken to search, \c std::unordered_set and \c std::set are virtually unbeatable under the conditions I've set for these tests.
The only way I could get \c self_organizing::lists to come ahead of was when I included the time taken to construct the container in the overall performance evaluation.
So that's something important to consider.

Performance tests were run with two random distributions, normal and geometric.
After the numbers were generated, an extra step was performed to substitute the generated numbers with values from a randomly generated substitution dictionary.
This avoided having the numbers generated being too predictable or concentrated.
For instance, for a geometric distribution, even though the distribution is random, the most common number generated will \em always be 0, the second will \em always be 1, etc.
Thus, this extra step added randomness to the numbers themselves, rather than just their distribution.

\subsection normal Search of normally distributed elements

For this performance test, I used the following containers:
 - One \c std::set, one \c std::unordered_set, one \c std::list and one \c std::vector.
 - Three self_organizing::list, each with a different \ref self_organizing::find_policy "find_policy".
 - Three self_organizing::vector, each with a different \ref self_organizing::find_policy "find_policy".

Each container was constructed with value from the range [0, 999999[.
I then generate 100000 random integers from the same range using a <A href=http://en.wikipedia.org/wiki/Normal_distribution>normal distribution</A>.
Each time, the mean was kept at 50000 and the variance was changed.
Then, the extra susbstitution step described above was applied.

\image html performance-normal.png

First obvious conclusion: \c std::set and \c std::unordered_set crush the competition.
They barely register on the graph.
\c std::set's <em>O(log(N))</em> search performance shines.
\c std::unordered_set's amortized constant performance fares even better, though it doesn't show on the graph.
Sorting or hashing the data even appears to have been done for free when compared to the other containers' performance.

On the other side of the spectrum, \c self_organizing::vector<find_policy::count> and \c self_organizing::vector<find_policy::move_to_front> can't keep up at all.
The maintenance required is utterly misadapted to a \c vector since both policies constanly rearrange elements using insertion, an operation that \c std::vector is not made for.

What's left in between?
In fourth place we have \c std::vector.
The nature of this test favors packed and static data, so that's no surprise.
But ahead of it by a nose is \c self_organizing::vector<find_policy::transpose>.
The transposition strategy of swapping element incurs little overhead to \c std::vector and afforded it the second place.

Where are our lists?
\c std::list and \c self_organizing::list<find_policy::transpose> performed in lock-step, mimicking their \c vector cousins.
Only, three times slower.
Remains, \c self_organizing::list<find_policy::count> and \c self_organizing::list<find_policy::move_to_front>.
Now those are interesting because they show a pattern of improving performance as the search variance is reduced.
\c self_organizing::list<find_policy::move_to_front> even managed to come ahead of the \c vectors in the last test!
Are self-organized lists vindicated?
Not quite.
To favor a self-organizing list as your container in this scenario, you would have to know in advance that your future searches will be performed on a very small subset of your data.
A variance of 1000 means that about two-thirds of searches will be performed on a window of 2000 elements.
It also implies that virtually all searches will be performed on a window of six times the variance value, or 6000.
Given our container size of 100000, it's a far cry from the "80/20" ratio.

But let's try to give our self-organizing containers a fighting chance...

\subsection normal-sorted Search of normally distributed and sorted elements

This next test is a carbon copy of the previous test except that the elements to search are sorted after they are generated.
The purpose is to imitate a system where it is expected that not only a small subset of elements will be searched for, but also that when an element is searched for it is highly likely to be immediately searched for again.
This situation will not help the transposition strategy but should help the move-to-front strategy.
Let's see.

\image html performance-normal-sorted.png

The picture is the same as before except for the performance of \c self_organizing::list<find_policy::move_to_front>.
It has benefited tremendeously from the search pattern, equating or besting the performance of \c std::vector for the smaller variance values.
But, it still doesn't hold a candle to either of \c std::set and \c std::unordered_set.
Let's up the ante.

\subsection geometric Search of geometrically distributed elements

Using knowledge from the previous tests, I narrowed the contenders to the following four:
 - One \c std::set
 - One \c std::unordered_set
 - One \c self_organizing::list<find_policy::count>
 - One \c self_organizing::list<find_policy::move_to_front>

Each container was constructed with value from the range [0, 999999[.
I then generated 100000 random integers multiple times from the same range using a < href=http://en.wikipedia.org/wiki/Geometric_distribution>geometric distribution</a> and varying \em p.
I measure both the time taken to contruct the container and the time taken to perform the searches twenty-five times to average out any outlying result.

\image html performance-geometric-100000.png

Self-organizing lists beat \c std::set! Victory!

Alas, it is a short-lived victory.
These values of \em p are ridiculously small.
At <em>p = 0.2</em> &mdash;the largest for which \c self_organizing::lists beat <tt>std::set</tt>&mdash; the variance is a measly <a href=http://www.wolframalpha.com/input/?i=GeometricDistribution[0.2]>20</a>.

Can we do better by making the sets do worse?
Consider that these tests measure both construction time and search time.
For \c std::set that means <em>O(Nlog(N))</em> construction performance and <em>O(log(N))</em> search performance.
Thus, if the size of the data set goes up by an order of magnitude, the overall performance of \c std::set should degrade proportionally.
As for \c std::unordered_set, it's construction performance is <em>O(N)</em> but it incurs hashing overhead.
So, our \c self_organizing::lists are at an advantage when considering construction performance but let's hope their search strategy can keep up.

\image html performance-geometric-1000000.png

With the size of the data set increased to one million, we managed to push \em p further down to 0.01, a variance of 9900.
Or 1% of our data set.
Meh.

So.
There you have it.
And I'm calling it a day.

\section sample Sample code

\include examples/example.cpp

\section license License

\verbatim
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
\endverbatim

\section references References

1. Clifford A. Shaffer. <EM>A practical Introduction to Data Structures and Algorithm Analysis.</EM> Prentice Hall, Upper Saddle River, NJ, 1997.

*/

