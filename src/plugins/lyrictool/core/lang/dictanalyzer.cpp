#include "dictanalyzer.h"
#include "dictanalyzer_p.h"

#include "multicharanalyzer.h"

#include <QtCore/QRandomGenerator>

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

    DictAnalyzerPrivate::DictAnalyzerPrivate() {
    }

    void DictAnalyzerPrivate::init() {
    }

    DictAnalyzer::DictAnalyzer(const QString &id, QObject *parent) : ILanguageAnalyzer(id, parent) {
    }

    void DictAnalyzer::loadDict() {
    }

    bool DictAnalyzer::contains(QChar c) const {
        Q_D(const DictAnalyzer);
        return d->trie.search(c);
    }

    bool DictAnalyzer::contains(const QString &input) const {
        Q_D(const DictAnalyzer);
        return d->trie.search(input);
    }

    QList<LyricInfo> DictAnalyzer::split(const QString &input) const {
        Q_D(const DictAnalyzer);

        QList<LyricInfo> result;

        int pos = 0;
        while (pos < input.length()) {
            const auto &currentChar = input[pos];
            LyricInfo note;

            if (d->trie.search(currentChar)) {
                const int start = pos;
                TrieNode currentNode = d->trie.root;

                // Greedily match the longest possible word
                for (int i = pos; i < input.length(); ++i) {
                    if (currentNode.children.contains(input[i])) {
                        currentNode = currentNode.children[input[i]];
                        if (currentNode.isEnd) {
                            pos++;
                        }
                    } else {
                        break;
                    }
                }

                if (pos > start) {
                    note.lyric() = input.mid(start, pos - start);
                    note.language() = id();
                    note.category() = category();
                } else {
                    note.lyric() = currentChar;
                    note.language() = QStringLiteral("Unknown");
                    note.category() = QStringLiteral("Unknown");
                    pos++;
                }
            } else {
                const int start = pos;
                while (pos < input.length() && !d->trie.search(input[pos])) {
                    pos++;
                }
                note.lyric() = input.mid(start, pos - start);
                note.language() = QStringLiteral("Unknown");
                note.category() = QStringLiteral("Unknown");
            }

            if (!note.lyric().isEmpty()) {
                result.append(note);
            }
        }

        return result;
    }

    QString DictAnalyzer::randString() const {
        Q_D(const DictAnalyzer);
        TrieNode node = d->trie.root;
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