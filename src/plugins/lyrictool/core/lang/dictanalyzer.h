#ifndef DICTANALYZER_H
#define DICTANALYZER_H

#include <lyrictool/ilanguageanalyzer.h>

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

    class LYRICTOOL_EXPORT DictAnalyzer : public ILanguageAnalyzer {
        Q_OBJECT
    public:
        DictAnalyzer(const QString &id, QObject *parent = nullptr);

        virtual void loadDict();

        bool contains(QChar c) const override;

        bool contains(const QString &input) const override;

        QString randString() const override;

    protected:
        Trie m_trie = Trie();
    };

}

#endif // DICTANALYZER_H
