#######################################################################
# SimDSP Project file.
#
# Copyright (c) 2017 lmcapacho
#
# This file is part of SimDSP.
#
# SimDSP is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SimDSP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SimDSP.  If not, see <http://www.gnu.org/licenses/>.
#
#######################################################################

QT       += core gui multimedia help

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

INCLUDEPATH += sdcore

LIBS += -Lsdcore/build -lsdcore

RESOURCES += simdspresources.qrc

include(pri/actions.pri)
include(pri/editor.pri)
include(pri/simdsp.pri)
include(pri/project.pri)

DESTDIR = build
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

TARGET = SimDSP
TEMPLATE = app
