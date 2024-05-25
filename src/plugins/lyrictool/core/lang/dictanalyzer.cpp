#include "dictanalyzer.h"
#include "multicharanalyzer.h"

#include <qrandom.h>

namespace LyricTool {
    Trie::Trie() {
        root = TrieNode();
    }

    Trie::~Trie() = default;

    void Trie::insert(const QString &word) const {
        TrieNode node = root;
        for (const auto &c : word) {
            if (!node.children.contains(c)) {
                node.children[c] = TrieNode();
            }
            node = node.children[c];
        }
        node.isEnd = true;
    }

    bool Trie::search(const QString &word) const {
        TrieNode node = root;
        for (const auto &c : word) {
            if (!node.children.contains(c)) {
                return false;
            }
            node = node.children[c];
        }
        return node.isEnd;
    }

    DictAnalyzer::DictAnalyzer(const QString &id, QObject *parent) : ILanguageAnalyzer(id, parent) {
    }

    void DictAnalyzer::loadDict() {
    }

    bool DictAnalyzer::contains(QChar c) const {
        return m_trie.search(c);
    }

    bool DictAnalyzer::contains(const QString &input) const {
        return m_trie.search(input);
    }

    QString DictAnalyzer::randString() const {
        TrieNode node = m_trie.root;
        QString word;
        while (true) {
            if (node.isEnd && QRandomGenerator::global()->bounded(2) == 0) {
                return word;
            }

            QList<QChar> keys = node.children.keys();
            if (keys.isEmpty()) {
                return word;
            }
            QChar randomChar = keys[QRandomGenerator::global()->bounded(keys.size())];
            node = node.children[randomChar];
            word.append(randomChar);
        }
    }

}