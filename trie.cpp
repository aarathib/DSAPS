
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node
{
    Node *letters[26];
    bool flag = false;

    bool containsKey(char ch)
    {
        return letters[ch - 'a'] != NULL;
    }

    void addNode(char ch, Node *node)
    {
        letters[ch - 'a'] = node;
    }

    Node *getNextNode(char ch)
    {
        return letters[ch - 'a'];
    }

    void setEnd()
    {
        flag = true;
    }

    bool isEnd()
    {
        return flag;
    }
};

class Trie
{
private:
    Node *root;

public:
    Trie()
    {
        root = new Node();
    }

    ~Trie()
    {
        delete[] root;
    }

    void insert(string word)
    {
        Node *node = root;
        for (int i = 0; i < word.length(); i++)
        {
            if (!node->containsKey(word[i]))
            {
                node->addNode(word[i], new Node());
            }
            node = node->getNextNode(word[i]);
        }
        node->setEnd();
    }

    bool search(string word)
    {
        Node *node = root;
        for (int i = 0; i < word.length(); i++)
        {
            if (!node->containsKey(word[i]))
            {
                return false;
            }
            node = node->getNextNode(word[i]);
        }
        return node->isEnd();
    }

    Node *startsWith(string prefix)
    {
        Node *node = root;
        for (int i = 0; i < prefix.length(); i++)
        {
            if (!node->isEnd() && !node->containsKey(prefix[i]))
            {
                return nullptr;
            }
            node = node->getNextNode(prefix[i]);
        }
        return node;
    }

    void getAutoCorrectWords(vector<string> &autocorrect, string t, string str, int index, Node *node, vector<int> &prev)
    {
        int x = t.size();
        if (node->isEnd())
        {
            if (prev[x] <= 3)

            {
                autocorrect.push_back(str);
            }
        }
        else
        {
            for (int i = 0; i < 26; i++)
            {
                if (node->containsKey('a' + i))
                {
                    char current_char = 'a' + i;
                    vector<int> curr(x + 1, 0);
                    curr[0] = index;
                    for (int j = 1; j <= x; j++)
                    {
                        if (current_char == t[j - 1])
                        {
                            curr[j] = prev[j - 1];
                        }
                        else
                        {
                            curr[j] = 1 + min(prev[j - 1], min(prev[j], curr[j - 1]));
                        }
                    }
                    getAutoCorrectWords(autocorrect, t, str + current_char, index + 1, node->getNextNode('a' + i), curr);
                }
            }
        }
    }

    void wordsAtEdit(vector<string> &autocorrect, string t)
    {
        vector<int> prev(t.size() + 1);
        for (int i = 0; i < t.size() + 1; i++)
        {
            prev[i] = i;
        }
        getAutoCorrectWords(autocorrect, t, "", 1, root, prev);
    }

    void getAllWords(vector<string> &words, Node *node, string t)
    {
        if (!node->isEnd())
        {
            for (int i = 0; i < 26; i++)
            {
                if (node->letters[i])
                {
                    string str = t + (char)('a' + i);
                    Node *ptr = node->letters[i];
                    getAllWords(words, ptr, str);
                }
            }
        }
        else
        {
            words.push_back(t);
        }
    }
};

int main()
{
    int n, q;
    cin >> n >> q;
    Trie trie;
    vector<pair<int, string>> queries;
    for (int i = 0; i < n; i++)
    {
        string word;
        cin >> word;
        trie.insert(word);
    }

    for (int i = 0; i < q; i++)
    {
        int a;
        string t;
        cin >> a;
        cin >> t;
        queries.push_back(make_pair(a, t));
    }
    for (int i = 0; i < q; i++)
    {
        int a = queries[i].first;
        string t = queries[i].second;
        switch (a)
        {
        case 1:
        {
            cout << trie.search(t) << "\n";
            break;
        }
        case 2:
        {
            Node *node = trie.startsWith(t);
            if (node == nullptr)
                cout << "0\n";
            else
            {
                vector<string> words;
                trie.getAllWords(words, node, t);
                cout << words.size() << "\n";
                for (int i = 0; i < words.size(); i++)
                {
                    cout << words[i] << "\n";
                }
            }
            break;
        }
        case 3:
        {
            vector<string> autocorrect;
            trie.wordsAtEdit(autocorrect, t);
            cout << autocorrect.size() << "\n";
            for (int i = 0; i < autocorrect.size(); i++)
            {
                cout << autocorrect[i] << "\n";
            }
        }
        }
    }

    return 0;
}
