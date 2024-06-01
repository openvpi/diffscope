#ifndef DICTANALYZER_P_H
#define DICTANALYZER_P_H

#include <lyrictool/dictanalyzer.h>
#include <lyrictool/private/ilanguageanalyzer_p.h>

namespace LyricTool {

    class TrieNode {
    public:
        QMap<QChar, TrieNode> children;
        bool isEnd;

        TrieNode() : isEnd(false) {
        }
    };

    class Trie {
    public:
        Trie();
        ~Trie();

        void insert(const QString &word) const;

        bool search(const QString &word) const;

        friend class DictAnalyzer;

    protected:
        TrieNode root;
        int depth = 0;
    };

    class DictAnalyzerPrivate : public ILanguageAnalyzerPrivate {
        Q_DECLARE_PUBLIC(DictAnalyzer)
    public:
        DictAnalyzerPrivate();

        void init();

        Trie trie = Trie();
    };

}

#endif // DICTANALYZER_P_H
