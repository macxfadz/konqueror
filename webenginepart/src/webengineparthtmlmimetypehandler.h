/*
 *    This file is part of the KDE project
 *    Copyright (C) 2018 Stefano Crocco <stefano.crocco@alice.it>
 * 
 *    This program is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU General Public License as
 *    published by the Free Software Foundation; either version 2 of
 *    the License or (at your option) version 3 or any later version
 *    accepted by the membership of KDE e.V. (or its successor approved
 *    by the membership of KDE e.V.), which shall act as a proxy
 *    defined in Section 14 of version 3 of the license.
 * 
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.
 * 
 *    You should have received a copy of the GNU Library General Public License
 *    along with this library; see the file COPYING.LIB.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
 * 
 */

#ifndef WEBENGINEPARTHTMLMIMETYPEHANDLER_H
#define WEBENGINEPARTHTMLMIMETYPEHANDLER_H

#include "utils.h"

#include <QWebEngineUrlSchemeHandler>
#include <QPointer>

class QWebEngineUrlRequestJob;
class QWebEnginePage;

/**
 * @brief Url scheme handler for URLs for which KIO produces HTML output
 * 
 * This class replies to the request with the HTML code produced by KIO. 
 * 
 * @internal
 * 
 * The issue is that html contains references to local resources (CSS, images)
 * and QtWebEngine refuses to load them because of cross-origin rules 
 * (URLs with scheme other than `file` aren't considered local and there's no way
 * to change this). To solve this problem, this class parses the HTML generated by KIO
 * using `QWebEnginePage`, then replaces URLs pointing to local files with their content
 * embedded as `data` URLs. Currently, this is only done for `link` (stylesheet) and `img` elements.
 * 
 * The main complications with this approach is that the to query the `QWebEnginePage` for
 * the HTML elements it contains one must use javascript and that the related functions
 * are all asynchronous. For this reason, we must rely on signals to tell when each step is done:
 * 
 * `QWebEnginePage::setHtml` -> `QWebEnginePage::loadFinished` -> startExtractingUrls() -> urlsExtracted() ->
 * startReplacingUrls() -> urlsReplaced() -> startRetrievingHtml() -> htmlRetrieved() -> sendReply()
 *
 * @note The `QWebEnginePage` used here is different from the one associated with the WebEnginePart.
 */
class WebEnginePartHtmlMimetypeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    
    /**
     * Constructor
     *
     * @param parent the parent object
     */
    WebEnginePartHtmlMimetypeHandler(QObject* parent = Q_NULLPTR);
    
    /**
    * @brief Destructor
    */
    ~WebEnginePartHtmlMimetypeHandler(){}
    
    /**
     * Implementation of `QWebEngineUrlSchemeHandler::requestStarted`
     *
     * @param job the information about the request
     */
    void requestStarted(QWebEngineUrlRequestJob* job) Q_DECL_OVERRIDE;
    
signals:
    
    /**
    * @brief Signal emitted when the javascript code which extracts `link` and `img` URLs finishes running
    * 
    * @param urls all the urls in `img` and `link` elements in the HTML code. The list contains URLs with any scheme and may contain duplicates
    */
    void urlsExtracted(const QStringList &urls);
    
    /**
    * @brief Signal emitted when the javascript code which replaces `file` URLs with `data` URLS in the HTML code finishes
    */
    void urlsReplaced();
    
    /**
    * @brief Signal emitted when the `QWebEnginePage` has returned its HTML code
    * 
    * @param html the HTML code of the page
    */
    void htmlRetrieved(const QString &html);
    
private slots:
    
    /**
    * @brief Extracts the Urls for `link` and `img` elements
    * 
    * This function is asynchronous, as it uses `QWebEnginePage::runJavascript`. urlsExtracted() will be emitted when the javascript code has run.
    * 
    * This function is called in response to the `loadFinished` signal of the internal `QWebEnginePage`
    */
    void startExtractingUrls();
    
    /**
    * @brief Replaces the `file` URLs in the list with `data` URLs
    * 
    * This function is asynchronous, as it uses `QWebEnginePage::runJavascript`. urlsReplaced() will be emitted when the javascript code has run.
    * 
    * This function is called in response to the urlsExtracted() signal
    * 
    * @param urls the URLs to replace. Only the appropriate ones (see dataUrl()) will be replaced.
    */
    void startReplacingUrls(const QStringList &urls);

    /**
    * @brief Sends the reply to the original URL request
    * 
    * This function assumes that `html` contains HTML (rather than XHTML) code.
    * 
    * This function is called in response to the htmlRetrieved() signal
    * 
    * @param html the HTML code to be sent as reply
    */
    void sendReply(const QString &html);
    
    /**
    * @brief Calls the `QWebEnginePage::toHtml` on the internal page
    * 
    * This function is asynchronous. htmlRetrieved() will be emitted when the HTML is ready
    * 
    * This function is called in response to the urlsReplaced() signal.
    */
    void startRetrievingHtml();
    
private:
    
    /**
    * @brief Converts an url to a `data` URL embeddding its contents
    * 
    * The conversion is performed only for URLs satisfying the following conditions:
    * 
    * * its scheme is `file`
    * * its filename is absolute
    * * the corresponding file can be opened in read mode.
    * 
    * If one of these conditions is not satisfied, an empty URL is returned
    * 
    * @param url the url to convert
    * @return the converted URL or an empty string if the URL can't be converted according to the rules above
    */
    QString dataUrl(const QUrl &url) const;
    
    /**
    * @brief The job requesting the URL
    * 
    */
    QPointer<QWebEngineUrlRequestJob> m_request;
    
    /**
    * @brief The `QWebEnginePage` used to parse the HTML code produced by kioslaves
    * 
    */
    QWebEnginePage *m_page;

};

#endif // WEBENGINEPARTHTMLMIMETYPEHANDLER_H
