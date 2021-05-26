#include "BBMainWindow.h"
#include "ui_BBMainWindow.h"
#include "BBUtils.h"
#include <QKeyEvent>


BBMainWindow::BBMainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_pUi(new Ui::BBMainWindow)
{
    m_pUi->setupUi(this);
    setWindowLayout();
    setGameObjectDockWidget();
    setConnect();
}

BBMainWindow::~BBMainWindow()
{
    BB_SAFE_DELETE(m_pUi);
}

void BBMainWindow::createProject()
{
//    //新项目预创建直射光
//    GameObject *object = ui->openGLWidget->scene.createLight("directional light.png", 650, 300);
//    //结点加入层级视图
//    ui->treeHierarchy->addGameObjectSlot(object);
//    //取消直射光的选中状态
//    ui->treeHierarchy->cancelSelectedItems();
}

void BBMainWindow::openProject()
{
//    //显示指定场景 或者新场景
//    newProject();
//    //加载层级视图 以及结点对应的场景对象

    // load file system
    m_pUi->treeFolder->loadProject();
//    //在文件列表中 显示contents里的文件
//    ui->listFile->showFolderContent(projectPath + contentsFolderName);
}

void BBMainWindow::setWindowLayout()
{
    // Allow nesting of docks
    setDockNestingEnabled(true);

    // dock layout
    addDockWidget(Qt::LeftDockWidgetArea, m_pUi->dockGameObject);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockEditview, Qt::Horizontal);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockInspector, Qt::Horizontal);
    splitDockWidget(m_pUi->dockGameObject, m_pUi->dockHierarchy, Qt::Vertical);
    splitDockWidget(m_pUi->dockEditview, m_pUi->dockProject, Qt::Vertical);
//    tabifyDockWidget(m_pUi->dockPreview, m_pUi->dockEditview);
//    tabifyDockWidget(m_pUi->dockConsole, m_pUi->dockProject);
//    splitDockWidget(m_pUi->dockProject, m_pUi->dockConsole, Qt::Horizontal);
//    m_pUi->dockPreview->setVisible(false);
//    m_pUi->dockConsole->setVisible(false);

    // resize
    m_pUi->dockGameObjectContents->updateSizeHint(QSize(350, 150));
    m_pUi->dockInspectorContents->updateSizeHint(QSize(300, 500));
    m_pUi->dockProjectContents->updateSizeHint(QSize(300, 250));

    // Stretch the width of the file list in the project view
    m_pUi->splitterProject->setStretchFactor(1, 1);
}

void BBMainWindow::setGameObjectDockWidget()
{
    // Load contents for list
    m_pUi->listBaseGameObject->loadListItems(BB_PATH_BASEOBJECTLIST);
    m_pUi->listBaseGameObject->setMimeType(BB_MIMETYPE_BASEOBJECT);
}

void BBMainWindow::setConnect()
{
    // global event about pressing Control key
    QObject::connect(this, SIGNAL(pressMultipleSelectionKey(bool)),
                     m_pUi->openGLWidget, SLOT(pressMultipleSelectionKey(bool)));
    // The handling of key events in the EditView in OpenGL
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressMoveKey(char)),
                     m_pUi->openGLWidget, SLOT(pressMoveKey(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(releaseMoveKey(char)),
                     m_pUi->openGLWidget, SLOT(releaseMoveKey(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressTransform(char)),
                     m_pUi->openGLWidget, SLOT(pressTransform(char)));
    QObject::connect(m_pUi->dockEditview, SIGNAL(pressESC()), m_pUi->openGLWidget, SLOT(pressESC()));
    QObject::connect(m_pUi->dockEditview, SIGNAL(keyPress(QKeyEvent*)), m_pUi->openGLWidget, SLOT(onKeyPress(QKeyEvent*)));
    // Drag into the model, and add item in the Hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(addGameObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(addGameObject(BBGameObject*)));
    // copy object in the scene, need to insert map and perform some operations in hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(addGameObject(BBGameObject*, QTreeWidgetItem*)),
                     m_pUi->treeHierarchy, SLOT(addGameObject(BBGameObject*, QTreeWidgetItem*)));
    // select item in treeHierarchy, use coordinate system to select corresponding object in OpenGL view
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(setCoordinateSystemSelectedObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(setCoordinateSystemSelectedObject(BBGameObject*)));
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(setCoordinateSystemSelectedObjects(QList<BBGameObject*>, BBGameObjectSet*)),
                     m_pUi->openGLWidget, SLOT(setCoordinateSystemSelectedObjects(QList<BBGameObject*>, BBGameObjectSet*)));
    // pick object in OpenGL view, select corresponding item in treeHierarchy
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObject(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(selectPickedItem(BBGameObject*)));
    QObject::connect(m_pUi->openGLWidget, SIGNAL(pickObjects(QList<BBGameObject*>)),
                     m_pUi->treeHierarchy, SLOT(selectPickedItems(QList<BBGameObject*>)));
    // press control to perform multiple selection in OpenGL view, also perform multiple selection in Hierarchy tree
    QObject::connect(m_pUi->openGLWidget, SIGNAL(updateMultipleSelectedObjects(BBGameObject*)),
                     m_pUi->treeHierarchy, SLOT(updateMultipleSelectedItems(BBGameObject*)));
    // click item in treeHierarchy, and then show property in inspector
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(showGameObjectProperty(BBGameObject*)),
                     m_pUi->propertyManager, SLOT(showGameObjectProperty(BBGameObject*)));
    // change transform in propertyManager, update coordinate system in OpenGL view
    QObject::connect(m_pUi->propertyManager, SIGNAL(coordinateSystemUpdated()),
                     m_pUi->openGLWidget, SLOT(updateCoordinateSystem()));
    // perform transform in OpenGL view, change transform in propertyManager
    QObject::connect(m_pUi->openGLWidget, SIGNAL(updateTransformInPropertyManager(BBGameObject*, char)),
                     m_pUi->propertyManager, SLOT(updateTransform(BBGameObject*, char)));
    // Drag files or prefabs into the hierarchical view, create model in the scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(createModel(QString)),
                     m_pUi->openGLWidget, SLOT(createModelAtOrigin(QString)));
    // perform operation in treeHierarchy, delete corresponding object in the scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(deleteGameObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(deleteGameObject(BBGameObject*)));
    // copy tree item in treeHierarchy, and copy game object in scene
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(copyGameObject(BBGameObject*, QTreeWidgetItem*)),
                     m_pUi->openGLWidget, SLOT(copyGameObject(BBGameObject*, QTreeWidgetItem*)));
    // double item, show close-up of corresponding gameobject
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(lookAtGameObject(BBGameObject*)),
                     m_pUi->openGLWidget, SLOT(lookAtGameObject(BBGameObject*)));
    // Select multiple items in the hierarchy tree, the property manager shows the properties of the set
    QObject::connect(m_pUi->treeHierarchy, SIGNAL(showGameObjectSetProperty(BBGameObject*, QList<BBGameObject*>)),
                     m_pUi->propertyManager, SLOT(showGameObjectSetProperty(BBGameObject*, QList<BBGameObject*>)));
    // click item in folder tree, show contents in file list
    QObject::connect(m_pUi->treeFolder, SIGNAL(showFolderContent(QString)),
                     m_pUi->listFile, SLOT(showFolderContent(QString)));
    // click buttons in the file system
    QObject::connect(m_pUi->buttonRootProject, SIGNAL(clicked()),
                     m_pUi->treeFolder, SLOT(pressRootButton()));
    QObject::connect(m_pUi->buttonMoreProject, SIGNAL(clicked()),
                     m_pUi->treeFolder, SLOT(pressSettingButton()));
    // show folder path in the bar
    QObject::connect(m_pUi->treeFolder, SIGNAL(showFolderContent(QString)),
                     m_pUi->barFilePath, SLOT(showCurrentFolderPath(QString)));
    QObject::connect(m_pUi->buttonMovePathLeft, SIGNAL(pressed()),
                     m_pUi->scrollAreaFilePath, SLOT(moveToLeft()));
    QObject::connect(m_pUi->buttonMovePathRight, SIGNAL(pressed()),
                     m_pUi->scrollAreaFilePath, SLOT(moveToRight()));
    // click an item in the file path bar, and change selected item in the folder tree
    QObject::connect(m_pUi->barFilePath, SIGNAL(showFolderContent(QString)),
                     m_pUi->treeFolder, SLOT(setCurrentItemByPath(QString)));


//    //对整个项目操作的菜单事件
//    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
//    //文件视图改变 需要刷新文件夹树
//    QObject::connect(ui->listFile, SIGNAL(updateProjectTree()), ui->treeProject, SLOT(loadProject()));

//    //文件列表跳转之后 文件夹树的currentShowFolderContentItem需要更新
//    QObject::connect(ui->listFile, SIGNAL(updateCurrentShowFolderContentItem(QString)),
//                     ui->treeProject, SLOT(updateCurrentShowFolderContentItem(QString)));
//    //在文件夹树中修改文件夹名字 当前文件夹路径更新 至文件列表
//    QObject::connect(ui->treeProject, SIGNAL(updateFolderName(QString, QString)),
//                     ui->listFile, SLOT(updateFolderName(QString, QString)));
//    //在文件列表中修改文件夹名字 树中相应项更新名字
//    QObject::connect(ui->listFile, SIGNAL(updateFolderNameInvert(QString, QString)),
//                     ui->treeProject, SLOT(updateFolderNameInvert(QString, QString)));
//    //在文件列表中创建文件夹 文件夹树添加相应项
//    QObject::connect(ui->listFile, SIGNAL(addItemInProjectTree(QString, QString)),
//                     ui->treeProject, SLOT(addFolderItemInTree(QString, QString)));
//    //在文件列表中删除文件夹 文件夹树相应项删去
//    QObject::connect(ui->listFile, SIGNAL(deleteItemInProjectTree(QString)),
//                     ui->treeProject, SLOT(deleteFolderItemInTree(QString)));
//    //在文件列表中移动文件夹 文件夹树相应项移动
//    QObject::connect(ui->listFile, SIGNAL(moveItemInProjectTree(QString, QString)),
//                     ui->treeProject, SLOT(moveFolderItemInTree(QString, QString)));
//    //从文件列表移动文件到文件夹树 删去移走项
//    QObject::connect(ui->treeProject, SIGNAL(deleteFileItemInList()), ui->listFile, SLOT(deleteItem()));
//    //文件列表菜单按钮 弹出菜单
//    QObject::connect(ui->buttonSettingFile, SIGNAL(clicked()), ui->listFile, SLOT(popupMenu()));
//    //主菜单的菜单项 根据控件的焦点调用不同控件的菜单项
//    QObject::connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(actionCopyByFocus()));
//    QObject::connect(ui->actionPaste, SIGNAL(triggered()), this, SLOT(actionPasteByFocus()));
//    QObject::connect(ui->actionDelete, SIGNAL(triggered()), this, SLOT(actionDeleteByFocus()));
//    //在文件列表中粘贴文件夹 文件夹树相应结点也要粘贴
//    QObject::connect(ui->listFile, SIGNAL(pasteItemInProjectTree(QList<QString>)),
//                     ui->treeProject, SLOT(pasteItemWithoutPasteFile(QList<QString>)));
//    //在文件夹树中复制 文件列表的剪贴板也改变
//    QObject::connect(ui->treeProject, SIGNAL(copyToFileList(QList<QString>)),
//                     ui->listFile, SLOT(copyByProjectTree(QList<QString>)));
//    //在文件列表中复制 文件夹树的剪贴板也改变
//    QObject::connect(ui->listFile, SIGNAL(copyToProjectTree(QList<QString>)),
//                     ui->treeProject, SLOT(copyByFileList(QList<QString>)));
//    //在文件夹树中粘贴 剪贴板上的非文件夹文件交给文件列表处理
//    QObject::connect(ui->treeProject, SIGNAL(pasteFile(QList<QString>, QString, QList<QString>)),
//                     ui->listFile, SLOT(pasteFileFromProjectTree(QList<QString>, QString, QList<QString>)));
//    //在文件列表中重命名 文件夹树对应的结点如果在剪贴板中 需要移除
//    QObject::connect(ui->listFile, SIGNAL(removeRenameItemInTreeClipBoard(QString)),
//                     ui->treeProject, SLOT(removeClipBoardRenameItem(QString)));
//    //属性栏根按钮切换
//    QObject::connect(ui->buttonRootInspector, SIGNAL(clicked()), this, SLOT(switchInspectorRootButton()));
//    //在层级视图中重命名 属性栏对应更新名字
//    QObject::connect(ui->treeHierarchy, SIGNAL(updateNameInInspector(GameObject*, QString)),
//                     ui->propertyManager, SLOT(renameGameObject(GameObject*, QString)));
//    //在属性栏更新名字 层级视图对应树节点重命名
//    QObject::connect(ui->propertyManager, SIGNAL(renameGameObjectInHierarchySignal(GameObject*)),
//                     ui->treeHierarchy, SLOT(renameItemName(GameObject*)));
//    //在属性栏勾选激活性 层级视图做相应处理
//    QObject::connect(ui->propertyManager, SIGNAL(changeActivationSignal(GameObject*, bool)),
//                     ui->treeHierarchy, SLOT(changeGameObjectActivation(GameObject*, bool)));
//    //层级视图修改激活性 属性栏对应图标更新
//    QObject::connect(ui->treeHierarchy, SIGNAL(changeButtonActiveCheckStateInInspector(GameObject*, bool)),
//                     ui->propertyManager, SLOT(changeButtonActiveCheckState(GameObject*, bool)));

//    //灯光拖入层级视图 在场景中创建灯光
//    QObject::connect(ui->treeHierarchy, SIGNAL(createLight(QString)),
//                     ui->openGLWidget, SLOT(createLightDependParent(QString)));
//    //在集合的属性栏中修改可见性 最后 重新选中所有对象 否则会选中最后一项
//    QObject::connect(ui->propertyManager, SIGNAL(setSelectedObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));

//    //在场景中框选的对象 在层级视图中选中
//    QObject::connect(ui->openGLWidget, SIGNAL(regionSelectObjects(QList<GameObject*>)),
//                     ui->treeHierarchy, SLOT(selectedObjects(QList<GameObject*>)));
//    //文件列表中选择 属性栏需要显示文件的属性 层级视图 场景 去掉选中项
//    QObject::connect(ui->listFile, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //层级视图 场景 去掉选中项
//    QObject::connect(ui->treeProject, SIGNAL(cancelHierarchyTreeSelectedItems()),
//                     ui->treeHierarchy, SLOT(cancelSelectedItems()));
//    //选中场景 或者 选中层级视图 或者拖预制体 文件列表去掉选中项
//    QObject::connect(ui->dockEditview, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->treeHierarchy, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listBaseGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    QObject::connect(ui->listLightGameObject, SIGNAL(cancelFileListSelectedItems()),
//                     ui->listFile, SLOT(cancelSelectedItems()));
//    //选中文件 在属性栏中显示文件的属性
//    QObject::connect(ui->listFile, SIGNAL(showMaterialProperty(QString)),
//                     ui->propertyManager, SLOT(showMaterialProperty(QString)));
//    QObject::connect(ui->listFile, SIGNAL(showFbxProperty(QString)),
//                     ui->propertyManager, SLOT(showFbxProperty(QString)));
//    //清空属性栏
//    QObject::connect(ui->listFile, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    QObject::connect(ui->treeProject, SIGNAL(clearPropertyWidget()), ui->propertyManager, SLOT(clear()));
//    //修改材质 文件列表的材质文件图标改变
//    QObject::connect(ui->propertyManager, SIGNAL(updateMaterialPreviewInOtherWidgetSignal(QString)),
//                     ui->listFile, SLOT(updateMaterialFileIcon(QString)));
//    //拖动材质 到场景中的对象 属性栏 对象的材质属性要更新
//    QObject::connect(ui->openGLWidget, SIGNAL(updateMaterialProperty(Model*)),
//                     ui->propertyManager, SLOT(updateMaterialFactory(Model*)));
//    //点击场景根按钮 属性栏显示场景属性
//    QObject::connect(ui->buttonRootHierarchy, SIGNAL(clicked()), this, SLOT(clickedButtonRootHierarchy()));

}

void BBMainWindow::keyPressEvent(QKeyEvent *e)
{
//    QMainWindow::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Control:
        pressMultipleSelectionKey(true);
        break;
    default:
        break;
    }
}

void BBMainWindow::keyReleaseEvent(QKeyEvent *e)
{
//    QMainWindow::keyReleaseEvent(e);

    switch (e->key()) {
    case Qt::Key_Control:
        pressMultipleSelectionKey(false);
        break;
    default:
        break;
    }
}
