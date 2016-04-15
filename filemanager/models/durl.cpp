#include "durl.h"

#include <QFileInfo>
#include <QSet>
#include <QDir>
#include <QDebug>

#define TRASH_SCHEME "trash"
#define RECENT_SCHEME "recent"
#define BOOKMARK_SCHEME "bookmark"
#define FILE_SCHEME "file"
#define COMPUTER_SCHEME "computer"
#define SEARCH_SCHEME "search"

QSet<QString> schemeList = QSet<QString>() << QString(TRASH_SCHEME)
                                           << QString(RECENT_SCHEME)
                                           << QString(BOOKMARK_SCHEME)
                                           << QString(FILE_SCHEME)
                                           << QString(COMPUTER_SCHEME )
                                           << QString(SEARCH_SCHEME);

DUrl::DUrl()
    : QUrl()
{

}

DUrl::DUrl(const QUrl &copy)
    : QUrl(copy)
{
    makeAbsolute();
}

#ifdef QT_NO_URL_CAST_FROM_STRING
ZUrl(const QString &url, ParsingMode mode)
    : QUrl(url, mode)
{
    makeAbsolute();
}
#else
DUrl::DUrl(const QString &url, QUrl::ParsingMode mode)
    : QUrl(url, mode)
{
    makeAbsolute();
}

void DUrl::setPath(const QString &path, QUrl::ParsingMode mode, bool makeAbsolute)
{
    QUrl::setPath(path, mode);

    if(makeAbsolute)
        this->makeAbsolute();
}

void DUrl::setScheme(const QString &scheme, bool makeAbsolute)
{
    QUrl::setScheme(scheme);

    if(makeAbsolute)
        this->makeAbsolute();
}

void DUrl::setUrl(const QString &url, QUrl::ParsingMode parsingMode, bool makeAbsolute)
{
    QUrl::setUrl(url, parsingMode);

    if(makeAbsolute)
        this->makeAbsolute();
}

bool DUrl::isTrashFile() const
{
    return scheme() == TRASH_SCHEME;
}

bool DUrl::isRecentFile() const
{
    return scheme() == RECENT_SCHEME;
}

bool DUrl::isBookMarkFile() const
{
    return scheme() == BOOKMARK_SCHEME;
}

bool DUrl::isSearchFile() const
{
    return scheme() == SEARCH_SCHEME;
}

bool DUrl::isComputerFile() const
{
    return scheme() == COMPUTER_SCHEME;
}

QString DUrl::toString(QUrl::FormattingOptions options) const
{
    if(isLocalFile() || !schemeList.contains(scheme()))
        return QUrl::toString(options);

    QUrl url(*this);

    url.setScheme(FILE_SCHEME);

    return url.toString(options).replace(0, 4, scheme());
}

DUrl DUrl::fromLocalFile(const QString &filePath)
{
    return QUrl::fromLocalFile(filePath);
}

DUrl DUrl::fromTrashFile(const QString &filePath)
{
    DUrl url;

    url.setScheme(TRASH_SCHEME, false);
    url.setPath(filePath);

    return url;
}

DUrl DUrl::fromRecentFile(const QString &filePath)
{
    DUrl url;

    url.setScheme(RECENT_SCHEME, false);
    url.setPath(filePath);

    return url;
}

DUrl DUrl::fromBookMarkFile(const QString &filePath)
{
    DUrl url;

    url.setScheme(BOOKMARK_SCHEME, false);
    url.setPath(filePath);

    return url;
}

DUrl DUrl::fromSearchFile(const QString &filePath)
{
    DUrl url;

    url.setScheme(SEARCH_SCHEME, false);
    url.setPath(filePath);

    return url;
}

DUrl DUrl::fromComputerFile(const QString &filePath)
{
    DUrl url;

    url.setScheme(COMPUTER_SCHEME, false);
    url.setPath(filePath);

    return url;
}

DUrlList DUrl::fromStringList(const QStringList &urls, QUrl::ParsingMode mode)
{
    QList<DUrl> urlList;

    for(const QString &string : urls) {
        urlList << DUrl(string, mode);
    }

    return urlList;
}

DUrlList DUrl::fromQUrlList(const QList<QUrl> &urls)
{
    QList<DUrl> urlList;

    for(const QUrl &url : urls) {
        urlList << url;
    }

    return urlList;
}

DUrl DUrl::fromUserInput(const QString &userInput)
{
    return fromUserInput(userInput, QString());
}

DUrl DUrl::fromUserInput(const QString &userInput, const QString &workingDirectory, QUrl::UserInputResolutionOptions options)
{
    if(userInput.startsWith("~")) {
        return QUrl::fromUserInput(QDir::homePath() + userInput.mid(1), workingDirectory, options);
    }

    DUrl url = QUrl::fromUserInput(userInput, workingDirectory, options);

    return url;
}

QStringList DUrl::toStringList(const DUrlList &urls, QUrl::FormattingOptions options)
{
    QStringList urlList;

    for(const DUrl &url : urls) {
        urlList << url.toString(options);
    }

    return urlList;
}

QList<QUrl> DUrl::toQUrlList(const DUrlList &urls)
{
    QList<QUrl> urlList;

    for(const DUrl &url : urls) {
        urlList << url;
    }

    return urlList;
}

bool DUrl::operator ==(const QUrl &url) const
{
    bool ok = QUrl::operator ==(url);

    if(ok || !schemeList.contains(url.scheme()))
        return ok;

    if(qAbs(this->toString().size() - url.toString().size()) != 1)
        return false;

    const QString &path1 = this->path();
    const QString &path2 = url.path();

    if(qAbs(path1.size() - path2.size()) != 1)
        return false;

    if(path1.size() > path2.size()) {
        QString tmp_str = path1;

        return tmp_str.remove(path2) == "/";
    } else {
        QString tmp_str = path2;

        return tmp_str.remove(path1) == "/";
    }
}

void DUrl::makeAbsolute()
{
    if(!schemeList.contains(this->scheme()))
        return;

    if(isLocalFile()) {
        const QString &path = this->path();

        if(path.startsWith("~"))
            QUrl::setPath(QDir::homePath() + path.mid(1));
        else
            QUrl::setPath(QFileInfo(path).absoluteFilePath());
    } else if(schemeList.contains(scheme())) {
        const QString &path = this->path();

        if(path.startsWith('/')) {
            QUrl::setPath(QFileInfo(this->path()).absoluteFilePath());
        }
    }
}
#endif

QT_BEGIN_NAMESPACE
QDebug operator<<(QDebug deg, const DUrl &url)
{
    QDebugStateSaver saver(deg);

    Q_UNUSED(saver)

    deg.nospace() << "DUrl(" << url.toString() << ")";

    return deg;
}
QT_END_NAMESPACE
