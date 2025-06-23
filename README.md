# 🃏 Console Solitaire Game (C++)

This is a simplified console-based **Solitaire** game implemented in C++ using stacks and a doubly linked list for card management. It supports card dealing, drawing from stock, moving between piles, and undoing moves.

## 📦 Features

- Standard 52-card deck with shuffling
- Card dealing into 7 tableau columns (c1–c7)
- Foundations (f1–f4), stock, and waste piles
- Move history tracking with undo support
- Face-up / face-down card display
- Clean, console-based interface

## 🕹️ Controls

Enter commands in the console to interact with the game:

| Command                  | Description                                              |
|--------------------------|----------------------------------------------------------|
| `s`                      | Draw a card from **Stock** to **Waste** pile             |
| `m <src> <dest> <num>`   | Move `<num>` cards from `<src>` to `<dest>`              |
| `z`                      | Undo the last move                                       |

### Examples:
- `s` – draw a card from the stock
- `m c3 f1 1` – move 1 card from column 3 to foundation 1
- `z` – undo the last move

## 🗂️ Valid Piles

- **Columns**: `c1` to `c7`
- **Foundations**: `f1` to `f4`
- **Stock & Waste**: `stock`, `waste`

> 📝 Note: Solitaire rules (e.g., alternating colors, correct order) are **not enforced**, so play fairly!

## 🚀 Getting Started

### Compile & Run
```bash
g++ Assignment1.cpp -o solitaire
./solitaire
