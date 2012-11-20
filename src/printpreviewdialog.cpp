#include "printpreviewdialog.h"
#include "ui_printpreviewdialog.h"

PrintPreviewDialog::PrintPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintPreviewDialog)
{
    ui->setupUi(this);
    printer = new QPrinter;
    ui->fontComboBox->setFont(ui->textEdit->font());
    ui->spinBoxFontSize->setValue(ui->textEdit->font().pointSize());

    // set default margins
    ui->comboBox->setCurrentIndex(0);//Inches
    ui->doubleSpinBoxLeft->setValue(0.75);
    ui->doubleSpinBoxTop->setValue(0.75);
    ui->doubleSpinBoxRight->setValue(0.75);
    ui->doubleSpinBoxBottom->setValue(0.75);
    updateMargins();

    // set default font
    QFont f;
    f.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    ui->fontComboBox->setCurrentFont(f);
}

PrintPreviewDialog::~PrintPreviewDialog()
{
    delete ui;
}

void PrintPreviewDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont font = f;
    font.setPointSize(ui->spinBoxFontSize->value());
    ui->textEdit->setFont(font);
}

void PrintPreviewDialog::on_spinBoxFontSize_valueChanged(int arg1)
{
    QFont font = ui->textEdit->font();
    font.setPointSize(arg1);
    ui->textEdit->setFont(font);
}

void PrintPreviewDialog::setText(Song song)
{
    QString s;
    s = QString("%1 %2\n%3\nTune: %4\nWords By: %5\tMusic By: %6\n\n")
            .arg(song.songbook_name).arg(song.number).arg(song.title).arg(song.tune).arg(song.wordsBy).arg(song.musicBy);
    song.songText=song.songText.split("@$").join("\n\n");
    song.songText=song.songText.split("@%").join("\n");
    s += QString("%1\n\nNotes:\n%2").arg(song.songText).arg(song.notes);

    qDebug();
    ui->textEdit->setText(s);
    ui->spinBoxFontSize->setValue(14);// default font size for Songs
}

void PrintPreviewDialog::on_pushButtonPDF_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this,tr("Save PDF as"),".","Portable Document Format(*.pdf)");
    if(filepath.isEmpty())
        return;
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(filepath);

    ui->textEdit->document()->print(printer);
}

void PrintPreviewDialog::on_pushButtonPrint_clicked()
{
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOutputFileName("");

    QPrintDialog *dlg = new QPrintDialog(printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    ui->textEdit->document()->print(printer);
}

void PrintPreviewDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    // set margin boxes
    double l,t,r,b;
    if(arg1 == "Inch")
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Inch);
    else if (arg1 == "Millimeter")
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Millimeter);
    else if (arg1 == "Pixel")
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::DevicePixel);
    else if (arg1 == "Point")
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Point);

    if(arg1 == "Inch")
    {
        ui->doubleSpinBoxLeft->setDecimals(2);
        ui->doubleSpinBoxLeft->setSingleStep(0.01);
        ui->doubleSpinBoxTop->setDecimals(2);
        ui->doubleSpinBoxTop->setSingleStep(0.01);
        ui->doubleSpinBoxRight->setDecimals(2);
        ui->doubleSpinBoxRight->setSingleStep(0.01);
        ui->doubleSpinBoxBottom->setDecimals(2);
        ui->doubleSpinBoxBottom->setSingleStep(0.01);
    }
    else
    {
        ui->doubleSpinBoxLeft->setDecimals(0);
        ui->doubleSpinBoxLeft->setSingleStep(1.0);
        ui->doubleSpinBoxTop->setDecimals(0);
        ui->doubleSpinBoxTop->setSingleStep(1.0);
        ui->doubleSpinBoxRight->setDecimals(0);
        ui->doubleSpinBoxRight->setSingleStep(1.0);
        ui->doubleSpinBoxBottom->setDecimals(0);
        ui->doubleSpinBoxBottom->setSingleStep(1.0);
    }

    ui->doubleSpinBoxLeft->setValue(l);
    ui->doubleSpinBoxTop->setValue(t);
    ui->doubleSpinBoxRight->setValue(r);
    ui->doubleSpinBoxBottom->setValue(b);
}

void PrintPreviewDialog::updateMargins()
{
    double l = ui->doubleSpinBoxLeft->value();
    double t = ui->doubleSpinBoxTop->value();
    double r = ui->doubleSpinBoxRight->value();
    double b = ui->doubleSpinBoxBottom->value();

    if(ui->comboBox->currentText() == "Inch")
        printer->setPageMargins(l,t,r,b,QPrinter::Inch);
    else if (ui->comboBox->currentText() == "Millimeter")
        printer->setPageMargins(l,t,r,b,QPrinter::Millimeter);
    else if (ui->comboBox->currentText() == "Pixel")
        printer->setPageMargins(l,t,r,b,QPrinter::DevicePixel);
    else if (ui->comboBox->currentText() == "Point")
        printer->setPageMargins(l,t,r,b,QPrinter::Point);
}

void PrintPreviewDialog::on_doubleSpinBoxLeft_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxTop_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxRight_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxBottom_editingFinished()
{
    updateMargins();
}
