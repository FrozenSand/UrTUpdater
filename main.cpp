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
  * @version    4.0.1
  * @author     Charles 'Barbatos' Duprey
  * @email      barbatos@urbanterror.info
  * @copyright  2013/2014 Frozen Sand / 0870760 B.C. Ltd
  */

#include "urtupdater.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UrTUpdater w;
    w.show();

    return a.exec();
}
