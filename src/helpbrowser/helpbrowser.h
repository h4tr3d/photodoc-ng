#ifndef HELPBROWSER_H
#define HELPBROWSER_H
//
#include <Qt>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QDialog>
#include <QApplication>
//
class HelpBrowser : public QDialog
{
Q_OBJECT
private slots:
    void updateCaption();

public:
    HelpBrowser(const QString& path,
                const QString& page,
                QWidget *parent = 0);
    ~HelpBrowser();

    static void showPage(const QString &page);

private:
    QVBoxLayout  *vblMain;
    QHBoxLayout  *hblButtons;
    QPushButton  *pbBack;
    QPushButton  *pbHome;
    QPushButton  *pbForward;
    QPushButton  *pbClose;
    QTextBrowser *txtBrowser;

};
#endif
