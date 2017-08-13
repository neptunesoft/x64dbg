#ifndef BREAKPOINTSVIEW_H
#define BREAKPOINTSVIEW_H

#include <QWidget>
#include "Bridge.h"
#include "StdTable.h"
#include "QBeaEngine.h"

class MenuBuilder;

class BreakpointsView : public StdTable
{
    Q_OBJECT
public:
    explicit BreakpointsView(QWidget* parent = 0);

protected:
    void setupContextMenu();
    void updateColors() override;
    void reloadData() override;
    QString paintContent(QPainter* painter, dsint rowBase, int rowOffset, int col, int x, int y, int w, int h) override;

private slots:
    void updateBreakpointsSlot();
    void disassembleAtSlot(dsint addr, dsint cip);
    void tokenizerConfigUpdatedSlot();
    void contextMenuSlot(const QPoint & pos);
    void followBreakpointSlot();
    void removeBreakpointSlot();
    void toggleBreakpointSlot();
    void editBreakpointSlot();
    void resetHitCountBreakpointSlot();
    void enableAllBreakpointsSlot();
    void disableAllBreakpointsSlot();
    void addDllBreakpointSlot();
    void addExceptionBreakpointSlot();

private:
    enum
    {
        ColType,
        ColAddr,
        ColModLabel,
        ColState,
        ColDisasm,
        ColHits,
        ColSummary
    };

    std::unordered_map<duint, const char*> mExceptionMap;
    std::vector<BRIDGEBP> mBps;
    std::vector<std::pair<RichTextPainter::List, RichTextPainter::List>> mRich;
    QColor mDisasmBackgroundColor;
    QColor mDisasmSelectionColor;
    QColor mCipBackgroundColor;
    QColor mCipColor;
    QColor mSummaryParenColor;
    QColor mSummaryKeywordColor;
    QColor mSummaryStringColor;
    duint mCip = 0;
    MenuBuilder* mMenuBuilder;
    QAction* mEnableDisableAction;
    QBeaEngine* mDisasm;

    const BRIDGEBP & selectedBp()
    {
        return mBps.at(getInitialSelection());
    }

    bool isValidBp()
    {
        return DbgIsDebugging() && !mBps.empty() && (selectedBp().addr != 0 || selectedBp().active);
    }

    QString bpTypeName(BPXTYPE type)
    {
        switch(type)
        {
        case bp_normal:
            return tr("Software");
        case bp_hardware:
            return tr("Hardware");
        case bp_memory:
            return tr("Memory");
        case bp_dll:
            return tr("DLL");
        case bp_exception:
            return tr("Exception");
        default:
            return QString();
        }
    }
};

#endif // BREAKPOINTSVIEW_H
