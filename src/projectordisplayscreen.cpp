#include "projectordisplayscreen.h"
#include "ui_projectordisplayscreen.h"

ProjectorDisplayScreen::ProjectorDisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectorDisplayScreen)
{
    ui->setupUi(this);
    dispView = new QQuickView;
    imProvider = new SpImageProvider;
    dispView->engine()->addImageProvider(QLatin1String("improvider"),imProvider);
    QWidget *w = QWidget::createWindowContainer(dispView,this);
    dispView->setSource(QUrl("qrc:/DisplayArea.qml"));
    ui->verticalLayout->addWidget(w);

    backImSwitch1 = backImSwitch2 = textImSwitch1 = textImSwitch2 = false;
    back1to2 = text1to2 = isNewBack = true;
    m_color.setRgb(0,0,0);// = QColor(QColor::black());
}

ProjectorDisplayScreen::~ProjectorDisplayScreen()
{
    delete ui;
}

void ProjectorDisplayScreen::resetImGenSize()
{
    imGen.setScreenSize(this->size());
}

void ProjectorDisplayScreen::setBackPixmap(QPixmap p, int fillMode)
{
    // fill mode -->>  0 = Strech, 1 = keep aspect, 2 = keep aspect by expanding
    
    if(back.cacheKey() == p.cacheKey())
    {
        isNewBack = false;
        return;
    }
    back = p;
    isNewBack = true;

    switch(fillMode)
    {
    case 0:
        p = p.scaled(imGen.getScreenSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        break;
    case 1:
        p = p.scaled(imGen.getScreenSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        break;
    case 2:
        p = p.scaled(imGen.getScreenSize(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        break;
    default:
        // Do No Scaling/resizing
        break;
    }

    imProvider->setPixMap(p);
    back1to2 = (!back1to2);

    QObject *item1 = dispView->rootObject()->findChild<QObject*>("backImage1");
    QObject *item2 = dispView->rootObject()->findChild<QObject*>("backImage2");
    if(item1 && item2)
    {
        if(back1to2)
        {
            backImSwitch2 = (!backImSwitch2);
            if(backImSwitch2)
                item2->setProperty("source","image://improvider/imB2a");
            else
                item2->setProperty("source","image://improvider/imB2b");

            if(p.height()<imGen.height())
                item2->setProperty("y",(imGen.height()-p.height())/2);
            else
                item2->setProperty("y",0);
            if(p.width()<imGen.width())
                item2->setProperty("x",(imGen.width()-p.width())/2);
            else
                item2->setProperty("x",0);
        }
        else
        {
            backImSwitch1 = (!backImSwitch1);
            if(backImSwitch1)
                item1->setProperty("source","image://improvider/imB1a");
            else
                item1->setProperty("source","image://improvider/imB1b");
            if(p.height()<imGen.height())
                item1->setProperty("y",(imGen.height()-p.height())/2);
            else
                item1->setProperty("y",0);
            if(p.width()<imGen.width())
                item1->setProperty("x",(imGen.width()-p.width())/2);
            else
                item1->setProperty("x",0);
        }
    }
}

void ProjectorDisplayScreen::setTextPixmap(QPixmap p)
{
    imProvider->setPixMap(p);
    text1to2 = (!text1to2);

    QObject *item1 = dispView->rootObject()->findChild<QObject*>("textImage1");
    QObject *item2 = dispView->rootObject()->findChild<QObject*>("textImage2");
    if(item1 && item2)
    {
        if(text1to2)
        {
            textImSwitch2 = (!textImSwitch2);
            if(textImSwitch2)
                item2->setProperty("source","image://improvider/imT2a");
            else
                item2->setProperty("source","image://improvider/imT2b");
        }
        else
        {
            textImSwitch1 = (!textImSwitch1);
            if(textImSwitch1)
                item1->setProperty("source","image://improvider/imT1a");
            else
                item1->setProperty("source","image://improvider/imT1b");
        }
    }
}

void ProjectorDisplayScreen::updateScreen()
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"stopTransitions");
    QString tranType = "seq";
    
    if(text1to2 && back1to2)
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack1to2",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText1to2",Q_ARG(QVariant,tranType));
    }
    else if(text1to2 && (!back1to2))
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack2to1",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText1to2",Q_ARG(QVariant,tranType));
    }
    else if((!text1to2) && back1to2)
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack1to2",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText2to1",Q_ARG(QVariant,tranType));
    }
    else
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack2to1",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText2to1",Q_ARG(QVariant,tranType));
    }
}

void ProjectorDisplayScreen::renderNotText()
{
    setTextPixmap(imGen.generateEmptyImage());
    updateScreen();
}

void ProjectorDisplayScreen::renderPassiveText(QPixmap &back, bool useBack)
{
    setTextPixmap(imGen.generateEmptyImage());
    if(useBack)
        setBackPixmap(back,0);
    else
        setBackPixmap(imGen.generateColorImage(m_color),0);
    
    updateScreen();
}

void ProjectorDisplayScreen::renderBibleText(Verse bVerse, BibleSettings &bSets)
{
    setTextPixmap(imGen.generateBibleImage(bVerse,bSets));
    if(bSets.useBackground)
        setBackPixmap(bSets.background,0);
    else
        setBackPixmap(imGen.generateColorImage(m_color),0);
    
    updateScreen();
}

void ProjectorDisplayScreen::renderSongText(Stanza stanza, SongSettings &sSets)
{
    setTextPixmap(imGen.generateSongImage(stanza,sSets));
    if(sSets.useBackground)
        setBackPixmap(sSets.background,0);
    else
        setBackPixmap(imGen.generateColorImage(m_color),0);
    
    updateScreen();
}

void ProjectorDisplayScreen::renderAnnounceText(AnnounceSlide announce, TextSettings &aSets)
{
    setTextPixmap(imGen.generateAnnounceImage(announce,aSets));
    if(aSets.useBackground)
        setBackPixmap(aSets.background,0);
    else
        setBackPixmap(imGen.generateColorImage(m_color),0);
    
    updateScreen();
}

void ProjectorDisplayScreen::renderSlideShow(QPixmap slide, SlideShowSettings &ssSets)
{
    bool expand;
    if(slide.width()<imGen.width() && slide.height()<imGen.height())
        expand = ssSets.expandSmall;
    else
        expand = true;

    setTextPixmap(imGen.generateEmptyImage());
    if(expand)
        setBackPixmap(slide,ssSets.fitType +1);
    else
        setBackPixmap(slide,3);
    updateScreen();

}
