# A Pseudo-Boolean Formulation for Graph Database Queries

> Pseudo-Boolean formulation for Graph Database Queries

[![Paper](https://img.shields.io/badge/Paper-PDF-blue)](LINK_TO_PAPER)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

---

## Overview

**Abstract:**  
Graph queries such as neighborhood exploration, friend-of-a-friend, and path finding are fundamental operations in graph databases. Traditionally, these queries are implemented using procedural traversal algorithms tightly coupled to the underlying graph structure. In this paper, we present a declarative formulation of graph database queries using pseudo-Boolean constraints. Vertices and edges are mapped to Boolean variables, and traversal semantics are enforced through cardinality constraints that regulate vertex roles and edge selection. The proposed formulation encodes first-degree queries, friend-of-a-friend queries, and shortest path queries as instances of constraint satisfaction or optimization problems. Preliminary experimental results indicate that the encoding is consistent and scales linearly with respect to the number of vertices and edges, although it is not yet competitive with specialized graph traversal algorithms in execution time. Nevertheless, the experimental analysis highlights clear directions for improvement, including solver specialization, incremental constraint generation, and optimized model enumeration, suggesting that pseudo-Boolean techniques can become a viable alternative for declarative graph querying.

**Contributions:**
- Bruno Ribas 
- Gabriel Tiveron

---

## Results

The average encoding time (base graph PB) and query-compilation time:

|\|V\|| base t (s) | query t (s) |
|-----|------------|-------------|
|100     | 0.0032  | 0.0000     |
|500     | 0.0176  | 0.0000     |
|1,000   | 0.0194  | 0.0000     |
|10,000  | 0.4746  | 0.0000     |

The average runtime to the shortest-path query:

|\|V\|| feasible model t(s) | optimal model t (s) |
|-----|---------------------|---------------------|
|50       | 0.0000 | 0.003 |
|100      | 0.0000 | 0.023 |
|200      | 0.0033 | 0.010 |
|500      | 0.0267 | 0.098 |
|1,000    | 0.1167 | 0.536 |
|10,000   | 1.3300 | 5.193 |

- **\|V\|**: Number of nodes;
- **feasible model**: Time to get the first feasible model;
- **optimal model**: Time to ensure the optimal model;

---

## Repository Structure
```
.
├─── README.md
├─── src/     # Core implementation.
├─── inc/     # Included header files.
└─── Makefile # Compilation instructions
```

---

## Installation

### Requirements
- C compiler (e.g., GCC ≥ 9.0 or Clang ≥ 10)
- GNU Make 3.81
- Python3 (for case generator)

### Setup

```bash
git clone https://github.com/UnB-SAT/pb-graph-query.git
cd pb-graph-query
make
```

## Usage

```bash
./gdb-sat.out < [path-to-input-file]
```

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt

python3 generator.py [number-of-nodes]
```

### Command-line arguments

The program accepts the following flags:

| Flag | Type | Default | Description |
|------|------|---------|-------------|
| `-v` | flag | off     | Enable detailed logging |
| `-b` | flag | off     | Enable benchmarking execution |
| `-t` | int  | 0       | Number of thread to use |
| `-d` | flag | off     | Enable debug logging |
| `-f` | string | none  | File name to be written |

### Command used for the paper
```bash
./gdb-sat.out -t 10 -b < [input-file]
```

