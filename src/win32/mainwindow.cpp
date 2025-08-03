#include "win32/mainwindow.h"

void MainWindow::onCreate(HWND hWnd)
{
	std::string installPath = "",
		installPathX64 = "";

	try {
		VMWareInfoRetriever vmwareInfo;
		installPath = vmwareInfo.getInstallPath();
		installPathX64 = vmwareInfo.getInstallPath64();
	} catch(const std::exception& exc)
	{ }

	char backupPath[MAX_PATH], toolsPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, backupPath);
	strcpy(toolsPath, backupPath);
	PathAppend(toolsPath, "tools");
	std::string toolsDefLoc(toolsPath);

	PathAppend(backupPath, "backup");
	DWORD dwBackupFolderAttrs = GetFileAttributes(backupPath);
	bool backupExists = (dwBackupFolderAttrs != INVALID_FILE_ATTRIBUTES) && (dwBackupFolderAttrs & FILE_ATTRIBUTE_DIRECTORY);

	// Add controls to the window
	browseLabel = std::make_unique<Label>(hWnd, NULL, "VMWare install location", 10, 10, 430, 15);
	pathEditBox = std::make_unique<EditBox>(hWnd, IDC_PATH_EDITBOX, installPath.c_str(), 10, 35, 300, 30);
	browseButton = std::make_unique<Button>(hWnd, IDC_PATH_BROWSEBTN, "Browse", 320, 35, 100, 30);
	browseLabelX64 = std::make_unique<Label>(hWnd, NULL, "VMWare X64 location", 10, 75, 430, 15);
	pathEditBoxX64 = std::make_unique<EditBox>(hWnd, IDC_PATH_EDITBOXX64, installPathX64.c_str(), 10, 100, 300, 30);
	browseButtonX64 = std::make_unique<Button>(hWnd, IDC_PATH_BROWSEBTNX64, "Browse", 320, 100, 100, 30);
	toolsGroup = std::make_unique<GroupBox>(hWnd, "Tools", 10, 140, 410, 135);
	downloadToolsChk = std::make_unique<CheckBox>(hWnd, IDC_DOWNLOADTOOLS_CHECKBOX, "Download tools", 20, 160, 400, 30);
	toolsPathLabel = std::make_unique<Label>(hWnd, NULL, "Existing tools location:", 20, 200, 400, 15);
	toolsPathEditBox = std::make_unique<EditBox>(hWnd, IDC_TOOLSPATH_EDITBOX, toolsDefLoc.c_str(), 20, 225, 255, 30);
	toolsBrowseBtn = std::make_unique<Button>(hWnd, IDC_TOOLSPATH_BROWSE_BTN, "Browse", 280, 225, 60, 30);
	downloadToolsBtn = std::make_unique<Button>(hWnd, IDC_TOOLS_DOWNLOAD_BTN, "Download", 340, 225, 70, 30);
	patchBtn = std::make_unique<Button>(hWnd, IDC_PATCH_BTN, "Patch", 10, 285, 100, 30);
	revertPatchBtn = std::make_unique<Button>(hWnd, IDC_REVERT_PATCH_BTN, "Uninstall patch", 115, 285, 100, 30);
	progressBar = std::make_unique<Progress>(hWnd, IDC_PROGRESSBAR, 10, 325, 410, 25);
	statusBar = std::make_unique<StatusBar>(hWnd, IDC_STATUSBAR, "Auto-Unlocker " PROG_VERSION);

	// Initial controls setup
	pathEditBox->setReadOnly(true);
	pathEditBoxX64->setReadOnly(true);
	toolsPathEditBox->setReadOnly(true);
	downloadToolsChk->set(true);
	toolsPathEditBox->setEnabled(false);
	revertPatchBtn->setEnabled(backupExists);
	progressBar->setRange(100);
	progressBar->setProgress(0);

	// Add listeners
	browseButton->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::browseButtonClick, this));
	browseButtonX64->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::browseX64ButtonClick, this));
	downloadToolsChk->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::downloadToolsChkClick, this));
	toolsBrowseBtn->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::toolsBrowseButtonClick, this));
	patchBtn->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::patchBtnClick, this));
	revertPatchBtn->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::revertPatchBtnClick, this));
	downloadToolsBtn->addEventListener(EventType::CLICK_EVENT, std::bind(&MainWindow::downloadToolsBtnClick, this));

	// Register controls for events dispatch
	registerControl(IDC_BROWSELABEL, browseLabel.get());
	registerControl(IDC_PATH_EDITBOX, pathEditBox.get());
	registerControl(IDC_PATH_BROWSEBTN, browseButton.get());
	registerControl(IDC_BROWSELABEL_X64, browseLabelX64.get());
	registerControl(IDC_PATH_EDITBOXX64, pathEditBoxX64.get());
	registerControl(IDC_PATH_BROWSEBTNX64, browseButtonX64.get());
	registerControl(IDC_TOOLS_GROUPBOX, toolsGroup.get());
	registerControl(IDC_DOWNLOADTOOLS_CHECKBOX, downloadToolsChk.get());
	registerControl(IDC_TOOLSPATH_EDITBOX, toolsPathEditBox.get());
	registerControl(IDC_TOOLSPATH_BROWSE_BTN, toolsBrowseBtn.get());
	registerControl(IDC_PATCH_BTN, patchBtn.get());
	registerControl(IDC_REVERT_PATCH_BTN, revertPatchBtn.get());
	registerControl(IDC_TOOLS_DOWNLOAD_BTN, downloadToolsBtn.get());
}

void MainWindow::browseButtonClick()
{
	char path[MAX_PATH];
	BROWSEINFO bInfo = {};
	bInfo.lpszTitle = "Select the VMWare install directory";
	bInfo.hwndOwner = hWnd;
	LPITEMIDLIST pIdl = SHBrowseForFolder(&bInfo);

	if (pIdl != 0) {
		SHGetPathFromIDList(pIdl, path);
		pathEditBox->setText(std::string(path));
	}
}

void MainWindow::browseX64ButtonClick()
{
	char path[MAX_PATH];
	BROWSEINFO bInfo = {};
	bInfo.lpszTitle = "Select the VMWare x64 install directory";
	bInfo.hwndOwner = hWnd;
	LPITEMIDLIST pIdl = SHBrowseForFolder(&bInfo);

	if (pIdl != 0) {
		SHGetPathFromIDList(pIdl, path);
		pathEditBoxX64->setText(std::string(path));
	}
}

void MainWindow::toolsBrowseButtonClick()
{
	char path[MAX_PATH];
	BROWSEINFO bInfo = {};
	bInfo.lpszTitle = "Select the tools folder (or where you want to download them)";
	bInfo.hwndOwner = hWnd;
	LPITEMIDLIST pIdl = SHBrowseForFolder(&bInfo);

	if (pIdl != 0) {
		SHGetPathFromIDList(pIdl, path);
		toolsPathEditBox->setText(std::string(path));
	}
}

void MainWindow::downloadToolsChkClick()
{
	bool isChecked = downloadToolsChk->isChecked();

	toolsPathEditBox->setEnabled(!isChecked);
}

void MainWindow::patchBtnClick()
{
	disableAllInput();

	patcherTask = new PatcherTask(*this);

	patcherTask->setOnProgressCallback(std::bind(&MainWindow::patchProgress, this, std::placeholders::_1));
	patcherTask->setOnCompleteCallback(std::bind(&MainWindow::patchComplete, this, std::placeholders::_1));

	patcherTask->run(nullptr);
}

void MainWindow::revertPatchBtnClick()
{
	disableAllInput();

	unpatcherTask = new UnpatcherTask(*this);

	unpatcherTask->setOnProgressCallback(std::bind(&MainWindow::patchProgress, this, std::placeholders::_1));
	unpatcherTask->setOnCompleteCallback(std::bind(&MainWindow::unpatchComplete, this, std::placeholders::_1));

	unpatcherTask->run(nullptr);
}

void MainWindow::downloadToolsBtnClick()
{
	disableAllInput();
	
	downloadToolsTask = new DownloadToolsTask(*this);

	downloadToolsTask->setOnProgressCallback(std::bind(&MainWindow::patchProgress, this, std::placeholders::_1));
	downloadToolsTask->setOnCompleteCallback(std::bind(&MainWindow::toolsDownloadComplete, this, std::placeholders::_1));

	downloadToolsTask->run(nullptr);
}

void MainWindow::disableAllInput()
{
	dlgState.pathBrowseEnabled = browseButton->isEnabled();
	browseButton->setEnabled(false);
	dlgState.pathEnabled = pathEditBox->isEnabled();
	pathEditBox->setEnabled(false);
	dlgState.path64BrowseEnabled = browseButtonX64->isEnabled();
	browseButtonX64->setEnabled(false);
	dlgState.path64Enabled = pathEditBoxX64->isEnabled();
	pathEditBoxX64->setEnabled(false);
	dlgState.downloadToolsChecked = downloadToolsChk->isChecked();
	dlgState.downloadToolsEnabled = downloadToolsChk->isEnabled();
	downloadToolsChk->setEnabled(false);
	dlgState.toolsPathEnabled = toolsPathEditBox->isEnabled();
	toolsPathEditBox->setEnabled(false);
	dlgState.toolsBrowseEnabled = toolsBrowseBtn->isEnabled();
	toolsBrowseBtn->setEnabled(false);
	dlgState.downloadToolsBtnEnabled = downloadToolsBtn->isEnabled();
	downloadToolsBtn->setEnabled(false);
	dlgState.patchEnabled = patchBtn->isEnabled();
	patchBtn->setEnabled(false);
	dlgState.unpatchEnabled = revertPatchBtn->isEnabled();
	revertPatchBtn->setEnabled(false);
}

void MainWindow::restoreInput()
{
	browseButton->setEnabled(dlgState.pathBrowseEnabled);
	pathEditBox->setEnabled(dlgState.pathEnabled);
	browseButtonX64->setEnabled(dlgState.path64BrowseEnabled);
	pathEditBoxX64->setEnabled(dlgState.path64Enabled);
	downloadToolsChk->set(dlgState.downloadToolsChecked);
	downloadToolsChk->setEnabled(dlgState.downloadToolsEnabled);
	toolsPathEditBox->setEnabled(dlgState.toolsPathEnabled);
	toolsBrowseBtn->setEnabled(dlgState.toolsBrowseEnabled);
	downloadToolsBtn->setEnabled(dlgState.downloadToolsBtnEnabled);
	patchBtn->setEnabled(dlgState.patchEnabled);
	revertPatchBtn->setEnabled(dlgState.unpatchEnabled);

	progressBar->setProgress(0);
}

void MainWindow::patchComplete(PatchResult result)
{
	if (result.result) {
		char msg[4096];
		sprintf(msg, "The patch has been installed successfully. Refer to the log file for detailed info:\n%s", result.logFilePath.c_str());
		MessageBox(hWnd, msg,
			"Success", MB_OK | MB_ICONINFORMATION);
	}
	else {
		// Check if this is the exact "already patched" error
		const std::string specificError = "The vmware installation you specified is already patched. Uninstall the previous patch first, or delete the .unlocker file (not suggested)";

		if (result.errorMessage == specificError) {
			// Create custom dialog with two buttons
			TASKDIALOGCONFIG config = { 0 };
			config.cbSize = sizeof(config);
			config.hwndParent = hWnd;
			config.dwFlags = TDF_POSITION_RELATIVE_TO_WINDOW;
			config.dwCommonButtons = TDCBF_OK_BUTTON;
			config.pszWindowTitle = L"Error";

			// Format error message exactly as in the original MessageBox
			std::string errorText = "An error occurred while applying the patch:\n" + 
				result.errorMessage + 
				"\nCheck the log file for detailed info:\n" + 
				result.logFilePath;

			// Convert to wide string for TaskDialog
			std::wstring wErrorMsg;
			wErrorMsg.assign(errorText.begin(), errorText.end());

			config.pszContent = wErrorMsg.c_str();
			config.pszMainIcon = TD_ERROR_ICON;

			// Custom button - "Delete .unlocker file"
			TASKDIALOG_BUTTON buttons[] = {
				{ 1000, L"Delete .unlocker file" }
			};
			config.pButtons = buttons;
			config.cButtons = ARRAYSIZE(buttons);

			int buttonPressed = 0;
			HRESULT hr = TaskDialogIndirect(&config, &buttonPressed, NULL, NULL);

			if (SUCCEEDED(hr) && buttonPressed == 1000) {
				// User clicked "Delete .unlocker file"
				try {
					fs::path vmwareInstallDir = fs::path(pathEditBox->getText());
					PatchVersioner patchVersion(vmwareInstallDir);
					patchVersion.removePatchVersion();

					// Show success message
					MessageBox(hWnd, "The .unlocker file has been successfully deleted. Restarting patch process...", 
						"File Deleted", MB_OK | MB_ICONINFORMATION);

					// Cleanup patcher task
					if (patcherTask != nullptr) {
						delete patcherTask;
						patcherTask = nullptr;
					}

					// Ensure UI is in the proper state before restarting
					restoreInput();

					// Restart patching process
					patchBtnClick();
					return; // Exit early since we're restarting the process
				}
				catch (const std::exception& exc) {
					MessageBox(hWnd, 
						("Failed to delete .unlocker file: " + std::string(exc.what())).c_str(),
						"Error", MB_OK | MB_ICONERROR);

					// Make sure to restore input controls when there's an error
					restoreInput();
				}
			}
		}
		else {
			char msg[4096];
			sprintf(msg, "An error occurred while applying the patch:\n%s\nCheck the log file for detailed info:\n%s", result.errorMessage.c_str(), result.logFilePath.c_str());
			MessageBox(hWnd, msg, "Error", MB_OK | MB_ICONERROR);
		}
	}

	if (patcherTask != nullptr)
	{
		delete patcherTask;
		patcherTask = nullptr;
	}

	restoreInput();
}

void MainWindow::unpatchComplete(PatchResult result)
{
	if (result.result) {
		char msg[4096];
		sprintf(msg, "The patch has been uninstalled successfully. Refer to the log file for detailed info:\n%s", result.logFilePath.c_str());
		MessageBox(hWnd, msg,
			"Success", MB_OK | MB_ICONINFORMATION);
	}
	else {
		char msg[4096];
		sprintf(msg, "An error occurred while uninstalling the patch:\n%s\nCheck the log file for detailed info:\n%s", result.errorMessage.c_str(), result.logFilePath.c_str());
		MessageBox(hWnd, msg, "Error", MB_OK | MB_ICONERROR);
	}

	if (unpatcherTask != nullptr)
	{
		delete unpatcherTask;
		unpatcherTask = nullptr;
	}

	restoreInput();
}

void MainWindow::toolsDownloadComplete(PatchResult result)
{
	if (result.result) {
		char msg[4096];
		sprintf(msg, "The tools have been downloaded successfully. Refer to the log file for detailed info:\n%s", result.logFilePath.c_str());
		MessageBox(hWnd, msg,
			"Success", MB_OK | MB_ICONINFORMATION);
	}
	else {
		char msg[4096];
		sprintf(msg, "An error occurred while downloading the tools:\n%s\nCheck the log file for detailed info:\n%s", result.errorMessage.c_str(), result.logFilePath.c_str());
		MessageBox(hWnd, msg, "Error", MB_OK | MB_ICONERROR);
	}

	if (downloadToolsTask != nullptr)
	{
		delete downloadToolsTask;
		downloadToolsTask = nullptr;
	}

	restoreInput();
}

void MainWindow::patchProgress(float progress)
{
	progressBar->setProgress((int)(progress * 100));
}

MainWindow::MainWindow(HINSTANCE hInstance, int nCmdShow)
	: Window(hInstance, nCmdShow, "auto-unlocker-mainwindow", "Auto-Unlocker " PROG_VERSION, CW_USEDEFAULT, CW_USEDEFAULT, 450, 420)
{
}

MainWindow::~MainWindow()
{
}
