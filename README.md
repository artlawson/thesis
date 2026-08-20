# Parallel Graphlet Census

Implementation and experimental results for a thesis on **graphlet census in large network structures**, with a focus on parallelizing graphlet enumeration using a **g-trie**.

## Key Result

Parallelizing graphlet enumeration across 32 threads reduced execution time from approximately 90 minutes to 7 minutes, achieving near-linear scaling through efficient work sharing.

## Overview

Graphlet census is a technique for characterizing the structure of a network by counting occurrences of a collection of small, connected subgraphs (graphlets). The resulting collection of counts can be viewed as a structural **fingerprint** of a network, making it possible to compare networks of similar size and identify differences in their local structure.

This thesis investigates algorithms for performing graphlet census efficiently on large networks. The work is based on the graphlet enumeration approach developed by Pedro Ribeiro and Fernando Silva and explores how the computation can be parallelized while maintaining efficient work sharing between processors.

The project implements three versions of the graphlet census algorithm:

1. **Sequential implementation** — a baseline implementation of the graphlet census algorithm.
2. **Parallel implementation** — distributes graphlet census work across multiple processors.
3. **Alternative parallel implementation** — explores a different strategy for dividing and sharing work between processors.

The primary goal is to investigate whether graphlet census can be effectively parallelized and to evaluate the performance and scalability of the resulting implementations.

## What is a Graphlet?

A graphlet is a small, connected, induced subgraph used as a building block for analyzing larger networks.

Rather than attempting to characterize an entire network directly, graphlet census counts how frequently different graphlets occur within it. For a given network, these counts form a vector:

```text
G = [g₁, g₂, g₃, ..., gₙ]
```

where each `gᵢ` represents the number of occurrences of a particular graphlet.

This vector can then be used as a structural signature or fingerprint for comparing networks.

## G-Trie

The core enumeration algorithm uses a **g-trie**, a tree-based data structure designed to efficiently store and search collections of graphs.

A g-trie shares common substructures between graphlets, allowing the graphlet enumeration algorithm to avoid repeatedly performing equivalent work. During census, the data structure is traversed while matching graphlets against the input network.

The implementation in this repository builds on the work of:

* Pedro Ribeiro
* Fernando Silva

and their research on graphlet census and g-trie-based graph mining.

## Implementations

The thesis explores three implementations of the graphlet census algorithm.

### Sequential

The sequential implementation provides a baseline against which the parallel implementations can be evaluated.

```text
Input Network
      │
      ▼
   G-Trie
      │
      ▼
Graphlet Enumeration
      │
      ▼
 Graphlet Counts
```

### Parallel

The first parallel implementation divides the graphlet census computation among multiple processors.

The objective is to reduce total execution time while keeping processors sufficiently busy and minimizing duplicated work.

### Parallel Work Sharing

The second parallel implementation explores a different approach to distributing graphlet census work.

Particular attention is given to **work sharing**, since graphlet enumeration can produce highly uneven workloads depending on the structure of the input network and the graphlets being searched.

## Repository Structure

The repository contains the thesis, source code, experimental material, and supporting files.

```text
.
├── code/                 # Source code for graphlet census
├── seq_p_code/           # Sequential / parallel implementation code
├── tex/                  # LaTeX source for the thesis
├── txt/                  # Text and supporting material
├── images/               # Figures and diagrams
├── thesis_history/       # Earlier versions / history of the thesis
├── good_notes_backupz/   # Research notes
├── good_notes_history/   # Historical research notes
└── aj318_Thesis.pdf      # Completed thesis
```

> The exact organization reflects the state of the research repository and includes both implementation and historical thesis material.

## Thesis

The complete thesis is available as:

**[Graphlet Census and Parallel Graphlet Enumeration](aj318_Thesis.pdf)**

The thesis describes the algorithms, implementation strategies, experimental methodology, and results in detail.

## Research Questions

The project investigates several related questions:

* How effectively can g-trie-based graphlet census be parallelized?
* How should graphlet census work be divided among processors?
* What impact does work distribution have on parallel performance?
* How does the parallel implementation compare with the sequential baseline?
* How well do the approaches scale as the size of the input network increases?

## Results

The experiments evaluate the sequential and parallel implementations with respect to execution time and the effectiveness of distributing graphlet census work across processors.

The thesis contains the detailed experimental results and discussion of the tradeoffs between the different approaches.

In particular, the results examine whether parallel execution can provide meaningful performance improvements and where the primary bottlenecks in graphlet enumeration occur.

## Background

This work builds on research into graphlet census and g-trie graph mining, particularly work by **Pedro Ribeiro and Fernando Silva**.

For additional background, see the references included in the thesis.

## Reproducibility

This repository contains the source code and research artifacts associated with the thesis. Because the project was developed as part of an academic research project, some components may require modification to run on modern systems.

The thesis PDF should be treated as the authoritative description of the algorithms and experiments.

## Author

**Art Lawson**

This repository contains the implementation and supporting material for my thesis research on parallel graphlet census.

---

### Keywords

`graph theory` · `graphlets` · `graphlet census` · `network analysis` · `g-trie` · `graph mining` · `parallel computing` · `high-performance computing` · `network motifs`
