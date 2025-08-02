# Cash Flow Minimizer

A C++ application that helps groups of people (like friends on a trip) minimize the number of transactions needed to settle all debts among themselves. Instead of having multiple complex transactions between different people, this tool finds the optimal way to settle all balances with the minimum number of payments.

## 🎯 Problem Solved

When groups of people share expenses (like on trips, shared apartments, or group events), tracking who owes what to whom can become complex. For example:
- Alice paid $100 for dinner (split 4 ways)
- Bob paid $50 for gas (split 2 ways) 
- Charlie paid $30 for snacks (split 3 ways)
- David paid $80 for hotel (split 4 ways)

Instead of having multiple small transactions, this tool calculates the net balance for each person and finds the minimum number of transactions needed to settle everything.

### 🚀 Key Efficiency Gain

The algorithm dramatically reduces the number of transactions needed:
- **Worst Case**: n-1 transactions (where n is the number of people)
- **Traditional Approach**: Would require (n)(n-1)/2 transactions (all possible pairs)
- **Efficiency Gain**: Reduces from O(n²) to O(n) transactions in the worst case

For example, with 5 people:
- Traditional approach: (5×4)/2 = 10 possible transactions
- Our algorithm: Maximum 4 transactions needed

## ✨ Features

- **Interactive Input**: Easy-to-use menu system for entering transactions
- **Flexible Transaction Types**:
  - Split expenses between all group members
  - Individual one-to-one transactions
  - Split expenses between specific subgroups
- **Dynamic Group Management**: Add new people to the group during transaction entry
- **Optimal Settlement**: Uses graph algorithms to minimize the number of transactions needed
- **Detailed Output**: Shows both original transactions and optimized settlement plan

## 🏗️ Architecture

The project is organized into several key components:

### Core Classes

- **`CashFlowMinimizer`**: Main application class that handles user interaction and orchestrates the minimization process
- **`Receipt`**: Represents individual input transactions with debtor codes, creditor code, amount, and description
- **`Transaction`**: Represents optimized transactions for final settlement
- **`Graph`**: Implements an adjacency matrix to track debts between all participants

### Key Algorithms

The minimization algorithm works by:
1. Building a graph where vertices represent people and edges represent debts
2. Calculating net balances for each person (total owed - total owed to them)
3. Iteratively matching the person with the largest debt to the person with the largest credit
4. Creating transactions to settle these balances until all net amounts are zero

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC, Clang, or MSVC)
- Make sure you have `g++` installed on your system

### Compilation

```bash
g++ -std=c++17 -Wall *.cpp -o cash-flow-minimizer
```

### Running the Application

```bash
./cash-flow-minimizer
```

## 📖 Usage Guide

### 1. Setup Phase
- Enter the number of people in your group
- Input each person's name

### 2. Transaction Entry
The application provides three options for entering transactions:

**Option A: Split between everyone**
- One person pays for something that benefits the entire group
- The cost is automatically split equally among all members

**Option B: Individual transaction**
- Direct payment between two specific people
- Useful for personal loans or specific arrangements

**Option C: Split between subgroup**
- Split expenses among a specific subset of the group
- Useful when not everyone benefits from a particular expense

### 3. Results
When you're done entering transactions, select option D to:
- View all original transactions entered
- See the optimized settlement plan with minimum transactions needed

## 📁 Project Structure

```
cash-flow-minimizer/
├── app.cpp                 # Main application entry point
├── cashFlowMinimizer.hpp   # Header file with class definitions
├── cashFlowMinimizer.cpp   # Main implementation of the cash flow minimizer
├── graph.hpp              # Graph class header for adjacency matrix
├── graph.cpp              # Graph implementation
└── README.md              # This file
```

## 🔧 Technical Details

### Data Structures Used
- **Adjacency Matrix**: Tracks debts between all pairs of people
- **Maps**: Store person names to unique codes and net balances
- **Vectors**: Store transaction history and optimized transactions

### Algorithm Complexity
- **Time Complexity**: O(n²) where n is the number of people
- **Space Complexity**: O(n²) for the adjacency matrix

### Key Methods
- `minimizeTransactions()`: Core algorithm that finds optimal settlement
- `getNetAmounts()`: Calculates net balance for each person
- `gameLoop()`: Main application loop for user interaction

## 💡 Example Usage

```
How many people are in your trip: 4
Enter name of person 1: Alice
Enter name of person 2: Bob
Enter name of person 3: Charlie
Enter name of person 4: David

Enter the letter for the option you want to select:
    (a) Split between everyone.
    (b) For an individual.
    (c) Split betweem multiple individuals (not everyone).
    (d) Get results.

a
Enter the creditor's name: Alice
Enter the amount: 100
Enter the description: Dinner
```

## 🤝 Contributing

This is a learning project demonstrating:
- Object-oriented design in C++
- Graph algorithms and data structures
- Interactive console applications
- Problem-solving for real-world scenarios

Feel free to explore the code and suggest improvements!

## 📝 License

This project is open source and available under the MIT License.

## 🎓 Learning Objectives

This project demonstrates:
- **Graph Theory**: Using adjacency matrices to model financial relationships
- **Algorithm Design**: Implementing optimization algorithms
- **C++ Programming**: Classes, templates, STL containers, and memory management
- **User Interface Design**: Creating intuitive console-based interactions
- **Problem Modeling**: Converting real-world problems into computational solutions 