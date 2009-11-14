/*
 *   Copyright (C) 2008-2009 by Patryk Połomski
 *   cykuss@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <QtGui>
#include <iostream>
#include "configdialog.h"


ConfigDialog::ConfigDialog()
{
    setWindowIcon( QIcon(":/images/hi16-app-qmoorie.png") );
    setWindowTitle(qApp->applicationName()  + " " + qApp->applicationVersion() + " - Ustawienia programu");

    warning = new QLabel;
    warning->setText("<b>Niektóre zmiany wymagają uruchomienia aplikacji ponownie.<b>");
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(112, 112));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(120);
    contentsWidget->setMinimumSize(120,300);
    contentsWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    contentsWidget->setResizeMode(QListView::Adjust);
    contentsWidget->setSpacing(12);

    pagesWidget = new QStackedWidget;
    confpage = new ConfigurationPage();
    pagesWidget->addWidget(confpage);
    //pagesWidget->addWidget(new UpdatePage);

    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Anuluj"));

    createIcons();
    contentsWidget->setCurrentRow(0);

    //QObject::connect(pathButton,SIGNAL(clicked()),this,SLOT(setDir()));
    QObject::connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(okButton, SIGNAL(clicked()),this,SLOT(saveConfigFile()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout -> addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(warning);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void ConfigDialog::createIcons()
{
    QListWidgetItem *configButton = new QListWidgetItem(contentsWidget);
    configButton->setIcon(QIcon(":/images/config.png"));
    configButton->setText(tr("Konfiguracja"));
    configButton->setTextAlignment(Qt::AlignHCenter);
    configButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *updateButton = new QListWidgetItem(contentsWidget);
    updateButton->setIcon(QIcon(":/images/update.png"));
    updateButton->setText(tr("Upload"));
    updateButton->setTextAlignment(Qt::AlignHCenter);
    updateButton->setFlags(Qt::NoItemFlags);

    QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
    queryButton->setIcon(QIcon(":/images/query.png"));
    queryButton->setText(tr("Inne"));
    queryButton->setTextAlignment(Qt::AlignHCenter);
    queryButton->setFlags(Qt::NoItemFlags);

    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current){
        current = previous;
    }
    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
void ConfigDialog::saveConfigFile()
{
 Zmienne().PATH = confpage->pathEdit->text();
 Zmienne().LLEVEL = confpage->lBox->value();
 Zmienne().DLEVEL = confpage->dBox->value();
 Zmienne().KSEGMENTS = confpage->kBox->isChecked();
 setConfigDir();
 QFile configFile("config.txt");
 if (configFile.open(QFile::WriteOnly))
  {
    QTextStream out(&configFile);
    out.setCodec("UTF-8");
    if(Zmienne().PATH == QDir::homePath()) out << "home";
    else
      if(!Zmienne().PATH.endsWith(QDir::separator())) out << Zmienne().PATH+QDir::separator();
      else out << Zmienne().PATH;
    out <<"\n";
    out << Zmienne().LLEVEL <<"\n";
    out << Zmienne().DLEVEL<<"\n";
    if(Zmienne().KSEGMENTS)
     out <<"ON";
      else out << "OFF";
    out <<"\n";
  }
  else
   {
    QMessageBox::warning(NULL, "QMoorie", "Nie można zapisać pliku konfiguracyjnego.", "OK");
   };
 repaint();
 this->close();
}
void ConfigDialog::setConfigDir()
{
 QDir::setCurrent(QDir::homePath ());
 QDir currentDir = QDir::current();
 currentDir.mkdir(".config/Qmoorie");
 currentDir.cd(".config/Qmoorie");
 QDir::setCurrent(currentDir.path());
}
