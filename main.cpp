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

        for (auto i : text) {
            text_code += huffmanCode[i];
        }
        return text_code;
    }

    static char Decode(Node* node, int& index, string code) {
        while (true) {
            if (node == nullptr) {
                return '\0';
            }
            if (node->left == nullptr && node->right == nullptr) {
                return node->ch;
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
            decode += Decode(tree, index, code);
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




int main() {
    vector<string> test_strings = {"aba", "abababab", "ABBBBAaaaAAab", "aaaaabbbbcccaas", "afkjd", "fkjds;flkj",
                      "ututututu", "qqsdqqqsdttq", "twioerutiorut", "jflkdsjfkdls"};

    for (int i = 0; i < test_strings.size(); ++i) {
        string text = test_strings[i];
        string code = Huffman::EncodeText(text);
        string decode = Huffman::DecodeText(Huffman::GetFrequencyText(text), code);
        cout << "Test " << i + 1 << ": " << (text == decode ? "ok" : "error") << endl;
    }
    return 0;
}
