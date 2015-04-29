/**
  * Urban Terror Updater
  *
  * This software is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This software is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this software; if not, write to the Free Software
  * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
  *
  * @version    4.0.2
  * @author     Charles 'Barbatos' Duprey
  * @email      barbatos@urbanterror.info
  * @copyright  2013-2015 Frozen Sand / 0870760 B.C. Ltd
  */

#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    void init();

    int getVersionIdByName(QString);
    int getServerIdByName(QString);
    int getEngineIdByName(QString);

    QString getVersionNameById(int);
    QString getServerNameById(int);
    QString getEngineNameById(int);

    QList<versionInfo_s>    versionsList;
    QList<serverInfo_s>     downloadServers;
    QList<engineInfo_s>     enginesList;

    int currentVersion;
    int currentServer;
    int currentEngine;

    int currentUpdateBehavior;

public slots:
    void okButtonClicked();

signals:
    void settingsUpdated(int, int, int, int);

private:
    QPushButton*    okButton;
    QPushButton*    cancelButton;
    QLabel*         selectLabel;

    QComboBox*      versionList;
    QComboBox*      serverList;
    QComboBox*      engineList;
    QComboBox*      updateBehaviorList;

    QLabel*         versionLabel;
    QLabel*         serverLabel;
    QLabel*         engineLabel;
    QLabel*         updateLabel;

};

#endif // SETTINGS_H
