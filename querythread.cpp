#include "querythread.h"

QueryThread::QueryThread(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<PkgItem*>("QVector<PkgItem*>");
}

void QueryThread::splitResults(QNetworkReply *reply, QueryMode queryMode)
{
    // Get All reply
    qDebug()<<this->thread();

    QByteArray buf = reply->readAll();

    // Convert to JSON
    QJsonParseError err;
    QJsonDocument docment = QJsonDocument::fromJson(buf, &err);

    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"Can't import json data.";
        exit(0);
    }
    else
    {   // "results" Array
        QJsonArray resultsArr = docment.object().value("results").toArray();
        if(queryMode == QueryMode::OnlyOfficial || ((queryMode == QueryMode::OfficialAndAur) && (_count == 1)))
        {
            for(auto item : resultsArr)
            {
                QJsonObject itemObj = item.toObject();

                QString pkgname = itemObj.value("pkgname").toString();

                qDebug()<<"pkgname:"<<pkgname;

                QString pkgbase = itemObj.value("pkgbase").toString();
                QString repo = itemObj.value("repo").toString();
                QString arch = itemObj.value("arch").toString();
                QString pkgver = itemObj.value("pkgver").toString();
                int pkgrel = itemObj.value("pkgrel").toInt();
                int epoch = itemObj.value("epoch").toInt();
                QString pkgdesc = itemObj.value("pkgdesc").toString();
                QString url = itemObj.value("url").toString();
                QString filename = itemObj.value("filename").toString();
                int compressed_size = itemObj.value("compressed_size").toInt();
                int installed_size = itemObj.value("installed_size").toInt();
                QString build_date = itemObj.value("build_date").toString();
                QString last_update = itemObj.value("last_update").toString();
                QString flag_date = itemObj.value("flag_date").toString();

                QVector<QString> maintainers;
                QJsonValue maintainersValue = itemObj.value("maintainers");
                if(maintainersValue.isArray())
                {
                    QJsonArray maintainersArr = maintainersValue.toArray();

                    for(auto maintainerItem : maintainersArr)
                    {
                        maintainers.push_back(maintainerItem.toString());
                    }
                }
                else
                    maintainers.push_back(maintainersValue.toString());

                QString packager = itemObj.value("packager").toString();

                QVector<QString> groups;
                QJsonValue groupsValue = itemObj.value("groups");
                if(groupsValue.isArray())
                {
                    QJsonArray groupsArr = groupsValue.toArray();

                    for(auto groupItem : groupsArr)
                    {
                        groups.push_back(groupItem.toString());
                    }
                }
                else
                    groups.push_back(groupsValue.toString());

                QVector<QString> licenses;
                QJsonValue licensesValue = itemObj.value("licenses");
                if(licensesValue.isArray())
                {
                    QJsonArray licensesArr = licensesValue.toArray();

                    for(auto licenseItem : licensesArr)
                    {
                        licenses.push_back(licenseItem.toString());
                    }
                }
                else
                    licenses.push_back(licensesValue.toString());

                QVector<QString> conflicts;
                QJsonValue conflictsValue = itemObj.value("conflicts");
                if(conflictsValue.isArray())
                {
                    QJsonArray conflictsArr = conflictsValue.toArray();

                    for(auto conflictItem : conflictsArr)
                    {
                        conflicts.push_back(conflictItem.toString());
                    }
                }
                else
                    conflicts.push_back(conflictsValue.toString());

                QVector<QString> provides;
                QJsonValue providesValue = itemObj.value("provides");
                if(providesValue.isArray())
                {
                    QJsonArray providesArr = providesValue.toArray();

                    for(auto provideItem : providesArr)
                    {
                        provides.push_back(provideItem.toString());
                    }
                }
                else
                    provides.push_back(providesValue.toString());

                QVector<QString> replaces;
                QJsonValue replacesValue = itemObj.value("replaces");
                if(replacesValue.isArray())
                {
                    QJsonArray replacesArr = replacesValue.toArray();

                    for(auto replaceItem : replacesArr)
                    {
                        replaces.push_back(replaceItem.toString());
                    }
                }
                else
                    replaces.push_back(replacesValue.toString());

                QVector<QString> depends;
                QJsonValue dependsValue = itemObj.value("depends");
                if(dependsValue.isArray())
                {
                    QJsonArray dependsArr = dependsValue.toArray();

                    for(auto dependItem : dependsArr)
                    {
                        depends.push_back(dependItem.toString());
                    }
                }
                else
                    depends.push_back(dependsValue.toString());

                QVector<QString> optdepends;
                QJsonValue optdependsValue = itemObj.value("optdepends");
                if(optdependsValue.isArray())
                {
                    QJsonArray optdependsArr = optdependsValue.toArray();

                    for(auto optdependItem : optdependsArr)
                    {
                        optdepends.push_back(optdependItem.toString());
                    }
                }
                else
                    optdepends.push_back(optdependsValue.toString());

                QVector<QString> makedepends;
                QJsonValue makedependsValue = itemObj.value("makedepends");
                if(makedependsValue.isArray())
                {
                    QJsonArray makedependsArr = makedependsValue.toArray();

                    for(auto makedependItem : makedependsArr)
                    {
                        makedepends.push_back(makedependItem.toString());
                    }
                }
                else
                    makedepends.push_back(makedependsValue.toString());

                QVector<QString> checkdepends;
                QJsonValue checkdependsValue = itemObj.value("ckeckdepends");
                if(checkdependsValue.isArray())
                {
                    QJsonArray checkdependsArr = checkdependsValue.toArray();

                    for(auto checkdependItem : checkdependsArr)
                    {
                        checkdepends.push_back(checkdependItem.toString());
                    }
                }
                else
                    checkdepends.push_back(checkdependsValue.toString());


                OfficialPkgItem *tmpItem
                        = new OfficialPkgItem(pkgname,pkgbase,arch,
                                              pkgver,pkgrel,epoch,
                                              pkgdesc,url,maintainers,
                                              licenses,conflicts,provides,
                                              replaces,depends,optdepends,
                                              makedepends,checkdepends,groups,
                                              repo,filename,compressed_size,
                                              installed_size,build_date,last_update,
                                              flag_date,packager);

                tmpItem->printInfo();

                result.push_back(tmpItem);
            }
        }

        if(queryMode == QueryMode::OnlyAur || ((queryMode == QueryMode::OfficialAndAur) && (_count == 2)))
        {
            for(auto item : resultsArr)
            {
                QJsonObject itemObj = item.toObject();

                qlonglong id = itemObj.value("ID").toInt();
                qlonglong pkgbaseid = itemObj.value("PackageBaseID").toInt();
                QString pkgname = itemObj.value("Name").toString();
                QString pkgbase = "";
                QString arch = "";
                QString pkgver = itemObj.value("Version").toString().section('-',0,0);
                int pkgrel = itemObj.value("Version").toString().section('-',1,1).toInt();
                int epoch = 1;
                QString pkgdesc = itemObj.value("Description").toString();
                QString url = itemObj.value("URL").toString();
                int numvotes = itemObj.value("NumVotes").toInt();
                double popularity = itemObj.value("Popularity").toDouble();
                QString outofdate = itemObj.value("OutOfDate").toString();
                qlonglong firstsubmitted = itemObj.value("FirstSubmitted").toInt();
                qlonglong lastmodified = itemObj.value("LastModified").toInt();
                QString urlpath = itemObj.value("URLPath").toString();

                QVector<QString> maintainers;
                QJsonValue maintainersValue = itemObj.value("Maintainers");
                if(maintainersValue.isArray())
                {
                    QJsonArray maintainersArr = maintainersValue.toArray();

                    for(auto maintainerItem : maintainersArr)
                    {
                        maintainers.push_back(maintainerItem.toString());
                    }
                }
                else
                    maintainers.push_back(maintainersValue.toString());

                QString packager = itemObj.value("packager").toString();

                QVector<QString> licenses;
                licenses.push_back("");

                QVector<QString> conflicts;
                conflicts.push_back("");

                QVector<QString> provides;
                provides.push_back("");

                QVector<QString> replaces;
                replaces.push_back("");

                QVector<QString> depends;
                depends.push_back("");

                QVector<QString> optdepends;
                optdepends.push_back("");

                QVector<QString> makedepends;
                makedepends.push_back("");

                QVector<QString> checkdepends;
                checkdepends.push_back("");

                QVector<QString> groups;
                groups.push_back("");

                QVector<QString> keywords;
                keywords.push_back("");


                AurPkgItem *tmpItem
                        = new AurPkgItem(pkgname,pkgbase,arch,
                                         pkgver,pkgrel,epoch,
                                         pkgdesc,url,maintainers,
                                         licenses,conflicts,provides,
                                         replaces,depends,optdepends,
                                         makedepends,checkdepends,groups,
                                         id,pkgbaseid,numvotes,
                                         popularity,outofdate,firstsubmitted,
                                         lastmodified,urlpath,keywords);

                tmpItem->printInfo();

                result.push_back(tmpItem);
            }

        }

        if(queryMode == QueryMode::AurInfo)
        {
            for(auto item : resultsArr)
            {
                QJsonObject itemObj = item.toObject();

                qlonglong id = itemObj.value("ID").toInt();
                qlonglong pkgbaseid = itemObj.value("PackageBaseID").toInt();
                QString pkgname = itemObj.value("Name").toString();
                QString pkgbase = "";
                QString arch = "";
                QString pkgver = itemObj.value("Version").toString().section('-',0,0);
                int pkgrel = itemObj.value("Version").toString().section('-',1,1).toInt();
                int epoch = 1;
                QString pkgdesc = itemObj.value("Description").toString();
                QString url = itemObj.value("URL").toString();
                int numvotes = itemObj.value("NumVotes").toInt();
                double popularity = itemObj.value("Popularity").toDouble();
                QString outofdate = itemObj.value("OutOfDate").toString();
                qlonglong firstsubmitted = itemObj.value("FirstSubmitted").toInt();
                qlonglong lastmodified = itemObj.value("LastModified").toInt();
                QString urlpath = itemObj.value("URLPath").toString();

                QVector<QString> maintainers;
                QJsonValue maintainersValue = itemObj.value("Maintainers");
                if(maintainersValue.isArray())
                {
                    QJsonArray maintainersArr = maintainersValue.toArray();

                    for(auto maintainerItem : maintainersArr)
                    {
                        maintainers.push_back(maintainerItem.toString());
                    }
                }
                else
                    maintainers.push_back(maintainersValue.toString());

                QString packager = itemObj.value("packager").toString();

                QVector<QString> licenses;
                QJsonValue licensesValue = itemObj.value("License");
                if(licensesValue.isArray())
                {
                    QJsonArray licensesArr = licensesValue.toArray();

                    for(auto licenseItem : licensesArr)
                    {
                        licenses.push_back(licenseItem.toString());
                    }
                }
                else
                    licenses.push_back(licensesValue.toString());

                QVector<QString> conflicts;
                QJsonValue conflictsValue = itemObj.value("Conflicts");
                if(conflictsValue.isArray())
                {
                    QJsonArray conflictsArr = conflictsValue.toArray();

                    for(auto conflictItem : conflictsArr)
                    {
                        conflicts.push_back(conflictItem.toString());
                    }
                }
                else
                    conflicts.push_back(conflictsValue.toString());

                QVector<QString> provides;
                QJsonValue providesValue = itemObj.value("Provides");
                if(providesValue.isArray())
                {
                    QJsonArray providesArr = providesValue.toArray();

                    for(auto provideItem : providesArr)
                    {
                        provides.push_back(provideItem.toString());
                    }
                }
                else
                    provides.push_back(providesValue.toString());

                QVector<QString> replaces;
                QJsonValue replacesValue = itemObj.value("Replaces");
                if(replacesValue.isArray())
                {
                    QJsonArray replacesArr = replacesValue.toArray();

                    for(auto replaceItem : replacesArr)
                    {
                        replaces.push_back(replaceItem.toString());
                    }
                }
                else
                    replaces.push_back(replacesValue.toString());

                QVector<QString> depends;
                QJsonValue dependsValue = itemObj.value("Depends");
                if(dependsValue.isArray())
                {
                    QJsonArray dependsArr = dependsValue.toArray();

                    for(auto dependItem : dependsArr)
                    {
                        depends.push_back(dependItem.toString());
                    }
                }
                else
                    depends.push_back(dependsValue.toString());

                QVector<QString> optdepends;
                QJsonValue optdependsValue = itemObj.value("OptDepends");
                if(optdependsValue.isArray())
                {
                    QJsonArray optdependsArr = optdependsValue.toArray();

                    for(auto optdependItem : optdependsArr)
                    {
                        optdepends.push_back(optdependItem.toString());
                    }
                }
                else
                    optdepends.push_back(optdependsValue.toString());

                QVector<QString> makedepends;
                QJsonValue makedependsValue = itemObj.value("MakeDepends");
                if(makedependsValue.isArray())
                {
                    QJsonArray makedependsArr = makedependsValue.toArray();

                    for(auto makedependItem : makedependsArr)
                    {
                        makedepends.push_back(makedependItem.toString());
                    }
                }
                else
                    makedepends.push_back(makedependsValue.toString());

                QVector<QString> checkdepends;
                QJsonValue checkdependsValue = itemObj.value("CheckDepends");
                if(checkdependsValue.isArray())
                {
                    QJsonArray checkdependsArr = checkdependsValue.toArray();

                    for(auto checkdependItem : checkdependsArr)
                    {
                        checkdepends.push_back(checkdependItem.toString());
                    }
                }
                else
                    checkdepends.push_back(checkdependsValue.toString());

                QVector<QString> groups;
                QJsonValue groupsValue = itemObj.value("Groups");
                if(groupsValue.isArray())
                {
                    QJsonArray groupsArr = groupsValue.toArray();

                    for(auto groupItem : groupsArr)
                    {
                        groups.push_back(groupItem.toString());
                    }
                }
                else
                    groups.push_back(groupsValue.toString());

                QVector<QString> keywords;
                QJsonValue keywordsValue = itemObj.value("Keywords");
                if(keywordsValue.isArray())
                {
                    QJsonArray keywordsArr = keywordsValue.toArray();

                    for(auto keywordItem : keywordsArr)
                    {
                        keywords.push_back(keywordItem.toString());
                    }
                }
                else
                    keywords.push_back(keywordsValue.toString());


                AurPkgItem *tmpItem
                        = new AurPkgItem(pkgname,pkgbase,arch,
                                         pkgver,pkgrel,epoch,
                                         pkgdesc,url,maintainers,
                                         licenses,conflicts,provides,
                                         replaces,depends,optdepends,
                                         makedepends,checkdepends,groups,
                                         id,pkgbaseid,numvotes,
                                         popularity,outofdate,firstsubmitted,
                                         lastmodified,urlpath,keywords);

                tmpItem->printInfo();

                result.push_back(tmpItem);
            }
        }
    }


    _count++;

    qDebug()<<"Splited";
}

void QueryThread::startQuery(QString target, QueryMode queryMode, MatchMode matchMode)
{

    // Emit started signal
    emit queryStarted();
    qDebug()<<"query thread started";
    qDebug()<<target<<" "<<queryMode<<" "<<matchMode;

    qDebug()<<this->thread();

    _count = 1;

    manager =  new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished,[=](QNetworkReply* reply){
        qDebug()<<"Query Finished";

        QNetworkReply::NetworkError err = reply->error();
        if(err != QNetworkReply::NoError) {
            qDebug() << "Failed: " << reply->errorString();
        }
        else
        {
            splitResults(reply, queryMode);



            if(queryMode == QueryMode::OfficialAndAur)
            {
                if(_count > 2)
                    emit queryFinished(result);
            }
            else
            {
                emit queryFinished(result);
            }
        }
    });


    if(queryMode == QueryMode::OnlyOfficial)
    {
        if(matchMode==MatchMode::NameAndDesc)
            request.setUrl(QUrl(tr("https://www.archlinux.org/packages/search/json/?q=%1").arg(target)));
        else
            request.setUrl(QUrl(tr("https://www.archlinux.org/packages/search/json/?name=%1").arg(target)));

        manager->get(request);
    }

    if(queryMode == QueryMode::OnlyAur)
    {
        if(matchMode == MatchMode::NameAndDesc)
            request.setUrl(QUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=search&arg=%1").arg(target)));
        else
            request.setUrl(QUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=search&by=name&arg=%1").arg(target)));

        manager->get(request);
    }

    if(queryMode == QueryMode::OfficialAndAur)
    {
        if(matchMode == MatchMode::NameAndDesc)
        {
            request.setUrl(QUrl(tr("https://www.archlinux.org/packages/search/json/?q=%1").arg(target)));
            manager->get(request);

            request.setUrl(QUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=search&arg=%1").arg(target)));
            manager->get(request);
        }
        else
        {
            request.setUrl(QUrl(tr("https://www.archlinux.org/packages/search/json/?name=%1").arg(target)));
            manager->get(request);

            request.setUrl(QUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=search&by=name&arg=%1").arg(target)));
            manager->get(request);
        }
    }

    if(queryMode == QueryMode::AurInfo)
    {
        request.setUrl(QUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=info&arg[]=%1").arg(target)));
        manager->get(request);
    }


}
