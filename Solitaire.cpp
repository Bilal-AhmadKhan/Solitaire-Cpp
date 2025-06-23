#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

// Doubly linked list node representing a card
struct Card {
   string suit;
   string rank;
   bool faceUp;   // Indicates if the card is face up (discovered) or face down
   Card* next;
   Card* prev;
};

// Doubly linked list for deck management
class Deck {
public:
   Card* head;
   Card* tail;

   Deck() : head(nullptr), tail(nullptr) {}

   // Create a standard deck of 52 cards
   void createDeck() {
       string suits[] = { "♠", "♥", "♦", "♣" };
       string ranks[] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

       for (const string& suit : suits) {
           for (const string& rank : ranks) {
               addCard(suit, rank);
           }
       }
   }

   // Add card to the doubly linked list
   void addCard(const string& suit, const string& rank) {
       Card* newCard = new Card{ suit, rank, false, nullptr, nullptr };
       if (!head) {
           head = tail = newCard;
       }
       else {
           tail->next = newCard;
           newCard->prev = tail;
           tail = newCard;
       }
   }

   // Shuffle the deck
   void shuffleDeck() {
       srand(time(0));
       vector<Card*> cardList;
       for (Card* current = head; current != nullptr; current = current->next) {
           cardList.push_back(current);
       }

       for (size_t i = 0; i < cardList.size(); ++i) {
           int randomIndex = rand() % cardList.size();
           swap(cardList[i], cardList[randomIndex]);
       }

       // Rebuild the linked list with shuffled cards
       head = cardList[0];
       head->prev = nullptr;
       Card* current = head;
       for (size_t i = 1; i < cardList.size(); ++i) {
           current->next = cardList[i];
           cardList[i]->prev = current;
           current = cardList[i];
       }
       tail = current;
       tail->next = nullptr;
   }

   // Deal a card from the head of the deck
   Card* dealCard() {
       if (!head) return nullptr;
       Card* dealtCard = head;
       head = head->next;
       if (head) head->prev = nullptr;
       return dealtCard;
   }
};

// Class representing a Solitaire game
class SolitaireGame {
private:
   Deck deck;
   stack<Card*> columns[7];    // 7 columns
   stack<Card*> foundations[4]; // 4 foundations
   stack<Card*> waste;          // Waste stack
   stack<Card*> stock;          // Stock stack
   vector<string> moveHistory;  // To store the move history for undo

   struct Move {
       string source;
       string destination;
       int num;
   };

   stack<Move> undoStack;  // Stores the moves for undo

public:
   SolitaireGame() {
       // Initialize deck
       deck.createDeck();
       deck.shuffleDeck();

       // Deal cards to columns
       dealToColumns();
   }

   // Deal cards to columns (simulating solitaire)
   void dealToColumns() {
       for (int i = 0; i < 7; ++i) {
           for (int j = 0; j <= i; ++j) {
               Card* dealtCard = deck.dealCard();
               if (j == i) {
                   dealtCard->faceUp = true; // Only the top card is face up
               }
               columns[i].push(dealtCard);
           }
       }

       // Remaining cards go to stock
       while (Card* card = deck.dealCard()) {
           stock.push(card);
       }
   }

   // Display the game layout
   void displayGame() {
       cout << "Stock   | Waste  | Foundation 1 | Foundation 2 | Foundation 3 | Foundation 4\n";
       cout << "--------|--------|--------------|--------------|--------------|--------------\n";
       printStockAndWaste();
       printFoundations();
       printColumns();
   }

   // Print the stock and waste piles
   void printStockAndWaste() {
       // Print Stock
       if (!stock.empty()) {
           cout << "[" << stock.top()->rank << stock.top()->suit << "]";
       }
       else {
           cout << "[ ]";
       }
       cout << "   |   ";

       // Print Waste
       if (!waste.empty()) {
           cout << "[" << waste.top()->rank << waste.top()->suit << "]";
       }
       else {
           cout << "[ ]";
       }
       cout << "   |   ";
   }

   // Print foundations
   void printFoundations() {
       for (int i = 0; i < 4; ++i) {
           if (!foundations[i].empty()) {
               cout << foundations[i].top()->rank << foundations[i].top()->suit;
           }
           else {
               cout << "[ ]";
           }
           if (i < 3) cout << "   |   ";
       }
       cout << endl;
   }

   // Print columns with hidden cards
   void printColumns() {
       cout << "\nColumns:\n";
       for (int i = 0; i < 7; ++i) {
           cout << "Column " << i + 1 << " (" << columns[i].size() << " cards): ";
           if (!columns[i].empty()) {
               printStack(columns[i]);
           }
           else {
               cout << "[ ]";
           }
           cout << endl;
       }
   }

   // Utility to print cards in a stack with face-up/face-down cards
   void printStack(stack<Card*> col) {
       vector<Card*> cards;
       while (!col.empty()) {
           cards.push_back(col.top());
           col.pop();
       }

       for (auto it = cards.rbegin(); it != cards.rend(); ++it) {
           if ((*it)->faceUp) {
               cout << (*it)->rank << (*it)->suit << " ";
           }
           else {
               cout << "[ ] ";
           }
       }
   }

   // Handle commands
   void handleCommand(string command) {
       stringstream ss(command);
       string token;
       ss >> token;

       if (token == "s") {
           drawCardFromStock();
       }
       else if (token == "m") {
           string source, destination;
           int num;
           ss >> source >> destination >> num;
           moveCard(source, destination, num);
       }
       else if (token == "z") {
           undoLastMove();
       }
       else {
           cout << "Invalid command" << endl;
       }

       displayGame();
   }

   // Draw card from stock to waste
   void drawCardFromStock() {
       if (!stock.empty()) {
           Card* card = stock.top();
           stock.pop();
           waste.push(card);
           moveHistory.push_back("draw");
           undoStack.push({ "stock", "waste", 1 });
       }
       else {
           cout << "Stock is empty!" << endl;
       }
   }

   // Move cards between piles
   void moveCard(string source, string destination, int num) {
       stack<Card*>* srcStack = getStack(source);
       stack<Card*>* destStack = getStack(destination);

       if (!srcStack || !destStack || srcStack->size() < num) {
           cout << "Invalid move!" << endl;
           return;
       }

       stack<Card*> tempStack;
       for (int i = 0; i < num; ++i) {
           tempStack.push(srcStack->top());
           srcStack->pop();
       }

       while (!tempStack.empty()) {
           destStack->push(tempStack.top());
           tempStack.pop();
       }

       undoStack.push({ source, destination, num });
       moveHistory.push_back("move " + source + " to " + destination);
   }

   // Undo the last move
   void undoLastMove() {
       if (undoStack.empty()) {
           cout << "No moves to undo!" << endl;
           return;
       }

       Move lastMove = undoStack.top();
       undoStack.pop();

       // Reverse the move
       stack<Card*>* srcStack = getStack(lastMove.destination);
       stack<Card*>* destStack = getStack(lastMove.source);

       if (!srcStack || !destStack || srcStack->size() < lastMove.num) {
           cout << "Undo failed!" << endl;
           return;
       }

       stack<Card*> tempStack;
       for (int i = 0; i < lastMove.num; ++i) {
           tempStack.push(srcStack->top());
           srcStack->pop();
       }

       while (!tempStack.empty()) {
           destStack->push(tempStack.top());
           tempStack.pop();
       }

       moveHistory.pop_back();
       cout << "Undo successful" << endl;
   }

   // Get the stack reference based on user input
   stack<Card*>* getStack(const string& label) {
       if (label == "waste") return &waste;
       if (label == "stock") return &stock;
       if (label == "f1") return &foundations[0];
       if (label == "f2") return &foundations[1];
       if (label == "f3") return &foundations[2];
       if (label == "f4") return &foundations[3];
       if (label[0] == 'c') {
           int colNum = stoi(label.substr(1)) - 1;
           if (colNum >= 0 && colNum < 7) return &columns[colNum];
       }
       return nullptr;
   }
};

int main() {
   SolitaireGame game;
   game.displayGame();

   string command;
   while (true) {
       cout << "Enter command (s = draw from stock, m = move, z = undo): ";
       getline(cin, command);
       game.handleCommand(command);
   }

   return 0;
}
