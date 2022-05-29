#pragma once

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QApplication>

class PushButtonDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PushButtonDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {};

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        auto editor = new QPushButton(parent);
        return editor;
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
     {
         QStyleOptionButton button;
         button.rect = option.rect;
         button.text = "Lap";
         button.state = QStyle::State_Enabled;

         QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
     }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override
    {
        editor->setGeometry(option.rect);
    }
};
