#include <bits/stdc++.h>
using namespace std;

enum OpType { ADD_WORD, DEL_WORD };

struct Op {
    OpType type;
    string word;
};

int main() {
    vector<string> doc;                 
    stack<Op> undoStack, redoStack;     
    string line;

    cout << "Console Text Editor (Undo/Redo one word at a time)\n";
    cout << "Commands:\n";
    cout << "WRITE <text>   : Append words (space-separated)\n";
    cout << "UNDO           : Undo last single-word change\n";
    cout << "REDO           : Redo last undone single-word change\n";
    cout << "READ           : Show current text\n";
    cout << "EXIT           : Quit\n";

    auto read_words = [&](const string& s) {
        istringstream iss(s);
        string w;
        vector<string> out;
        while (iss >> w) out.push_back(w);
        return out;
    };

    while (true) {
        cout << "\n> ";
        if (!getline(cin, line)) break;
        if (line.rfind("WRITE", 0) == 0) {
            string payload = (line.size() > 6 ? line.substr(6) : "");
            vector<string> words = read_words(payload);
            
            for (const string& w : words) {
                doc.push_back(w);
                undoStack.push({ADD_WORD, w});
                while (!redoStack.empty()) redoStack.pop();
            }
        } else if (line == "UNDO") {
            if (undoStack.empty()) {
                cout << "Nothing to undo!\n";
                continue;
            }
            Op op = undoStack.top(); undoStack.pop();
            if (op.type == ADD_WORD) {
                if (!doc.empty() && doc.back() == op.word) {
                    doc.pop_back();
                    redoStack.push(op); 
                } else {
                    if (!doc.empty()) doc.pop_back();
                    redoStack.push({ADD_WORD, op.word});
                }
            } else if (op.type == DEL_WORD) {
                doc.push_back(op.word);
                redoStack.push(op);
            }
        } else if (line == "REDO") {
            if (redoStack.empty()) {
                cout << "Nothing to redo!\n";
                continue;
            }
            Op op = redoStack.top(); redoStack.pop();
            if (op.type == ADD_WORD) {
                doc.push_back(op.word);
                undoStack.push(op);
            } else if (op.type == DEL_WORD) {
                if (!doc.empty() && doc.back() == op.word) {
                    doc.pop_back();
                    undoStack.push(op);
                } else if (!doc.empty()) {
                    doc.pop_back();
                    undoStack.push({DEL_WORD, op.word});
                }
            }
        } else if (line == "READ") {
            cout << "Current Text: ";
            for (size_t i = 0; i < doc.size(); ++i) {
                if (i) cout << ' ';
                cout << doc[i];
            }
            cout << '\n';
        } else if (line == "EXIT") {
            break;
        } else {
            cout << "Invalid command.\n";
        }
    }
    return 0;
}
