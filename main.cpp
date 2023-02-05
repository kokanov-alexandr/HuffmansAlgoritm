#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

struct Node {
    char ch;
    int frequency;
    Node* left;
    Node* right;
    Node(char ch, int frequency, Node* left, Node* right) {
        this->ch = ch;
        this->frequency = frequency;
        this->left = left;
        this->right = right;
    }
};

struct comp {
    bool operator()(const Node* first, const Node* second) {
        return first->frequency > second->frequency;
    }
};

class Huffman {
public:
    static unordered_map<char, int> GetFrequencyText(string text) {
        unordered_map<char, int> freq_map;
        for (auto ch : text) {
            freq_map[ch]++;
        }
        return freq_map;
    }

    static unordered_map<char, int> ScanFrequencyText() {
        unordered_map<char, int> freq_map;
        int n, b;
        char ch;
        cout << "Enter the alphabet size:" << endl;
        cin >> n;
        cout << "Enter letters and frequency (example: A 14):" << endl;
        for (int i = 0; i < n; ++i) {
            cin >> ch >> b;
            freq_map[ch] = b;
        }
        return freq_map;
    }

    static void Encode(Node* node, string code, unordered_map<char, string> &huffmanCode) {
        if (node == nullptr) {
            return;
        }
        if (node->left == nullptr && node->right == nullptr) {
            huffmanCode[node->ch] = code;
            if (code == "") {
                huffmanCode[node->ch] = "1";
            }
            return;
        }
        Encode(node->left, code + "0", huffmanCode);
        Encode(node->right, code + "1", huffmanCode);
    }

    static string EncodeText(string text) {
        unordered_map<char, string> huffmanCode;
        auto tree = BuildHuffmanTree(GetFrequencyText(text));
        Encode(tree, "", huffmanCode);
        string text_code = "";
//        for (auto i : huffmanCode) {
//            cout << i.first << " " << i.second << endl;
//        }
        for (auto i : text) {
            text_code += huffmanCode[i];
        }
        return text_code;
    }

    static void Decode(Node* node, int& index, string code, string& decode) {
        while (true) {
            if (node == nullptr) {
                return;
            }
            if (node->left == nullptr && node->right == nullptr) {
                decode += node->ch;
                return;
            }
            index++;
            if (code[index] == '0') {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
    }

    static string DecodeText(unordered_map<char, int> freq_map, const string& code) {
        auto tree = BuildHuffmanTree(freq_map);
        int index = -1;
        string decode = "";
        while (index < (int)code.size() - 1) {
            Decode(tree, index, code, decode);
            if (tree->left == nullptr && tree->right == nullptr) {
                index++;
            }
        }
        return decode;

    }


    static Node* BuildHuffmanTree(unordered_map<char, int> freq_map) {
        priority_queue<Node*, vector<Node*>, comp> tree;

        for (auto i : freq_map) {
            tree.push(new Node(i.first, i.second, nullptr, nullptr));
        }

        while (tree.size() != 1) {
            auto left = tree.top();
            tree.pop();
            auto right = tree.top();
            tree.pop();
            tree.push(new Node('\0', left->frequency + right->frequency, left, right));
        }
        return tree.top();
    }
};

void RunTests() {
    int CountCorrectTests = 0;
    int CountFailedTests = 0;
    vector<string> test_strings = {"aaa", "abababab", "ABBBBAaaaAAab", "aaaaabbbbcccaas", "afkjd", "fkjds;flkj",
                                   "ututututu", "qqsdqqqsdttq", "twioerutiorut", "jflkdsjfkdls", "FKLJFWIEOFEIOFJEWFKJHEKj1212"};

    for (int i = 0; i < test_strings.size(); ++i) {
        string text = test_strings[i];
        string code = Huffman::EncodeText(text);
        string decode = Huffman::DecodeText(Huffman::GetFrequencyText(text), code);
        cout << "Test " << i + 1 << ": " << text << endl;
        if (text == decode) {
            CountCorrectTests++;
            cout << "ok";
        }
        else {
            CountFailedTests++;
            cout << "error";
        }
        cout << endl;
    }
    cout << "Correct tests: " << CountCorrectTests << endl;
    cout << "Failed tests: " << CountFailedTests << endl << endl;
}
int main() {
    RunTests();
    string text;
    cout << "Enter text: \n";
    getline(cin, text);
    string code = Huffman::EncodeText(text);
    string decode = Huffman::DecodeText(Huffman::GetFrequencyText(text), code);
    cout << "Text: " << text << endl;
    cout << "Code: " << code << endl;
    cout << "Decode: " << decode << endl;
    return 0;
}
