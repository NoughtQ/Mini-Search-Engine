#import "../template/template.typ": *

#show: project.with(
  theme: "project",
  course: "Advanced Data Structures and Algorithm Analysis",
  title: "Project 1: Roll Your Own Mini Search Engine",
  cover_image_size: 50%,
  semester: "2024-2025 Autumn&Winter",
  author: none,
  school_id: "",
  date: "2024-09-30",
  table_of_contents: true
)

= Chapter 1: Introduction

== Problem Description

The project required us to create a *mini search engine* which can handle inquiries over "The Complete Works of William Shakespeare".

Here are some specific requirements:

- Run a word count over the Shakespeare set, extract all words from the documents by word stemming and try to identify the stop words.

- Create a customized inverted index over the Shakespeare set with word stemming. The stop words identified must not be included.

- Write a query program on top of the inverted file index, which will accept a user-specified word (or phrase) and return the IDs of the documents that contain that word.

- Run tests to show how the thresholds on query may affect the results.

#v(1em)

== Purpose of Report

#v(.5em)

- Show the details of the implementation of the mini search engine by showcasing essential data structures and algorithms.

- Demonstrate the correctness and efficiency of the program by analysis based on testing data and diagrams.

- Summarize the whole project, analyze the pros and cons of the mini search engine, and put forward the prospect of further improvement.

#v(1em)

== Backgound of Data Structures and Algorithms

#v(.5em)

1. *B+ Trees*: It's a improved version of search trees, widely used in the relational database and file management in operating systems. We will use this data structure to store and access to the inverted index.
2. 

#v(1.5em)

= Chapter 2: Data Structure / Algorithm Specification

== Algorithm Architecture

The overall algorithm architecture in the program is shown below:

In the following sections, I will introduce these algorithms from top to down, but with some slight adjustement, in the hope that you can gain a deeper insight into my whole program.

== The Main Program

== Word Count

== Word Stemming 

== Stop Words

== Inversed Index

== Query

= Chapter 3: Testing Results

== Test 1: Inverted Index

== Test 2: Thresholds for Queries

== Test 3: Speed Test

== (Maybe)Debug Mode

= Chapter 4: Analysis comments

== Time Complexity

== Space Complexity

= Appendix: Source code

== File Structure

= References

= Declaration

#v(.5em)

#fakebold[#fakeitalic[We hereby declare that all the work done in this project titled "Roll Your Own Mini Search Engine" is of our independent effort as a group.]]

