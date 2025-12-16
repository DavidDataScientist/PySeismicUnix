#!/usr/bin/env python3
"""
SU Flow GUI - A PyQt6 interface for developing and running SU workflows

This GUI provides:
- Dark mode interface
- Scrollable console output
- Flow entry with syntax highlighting hints
- Preset test workflows
- Integration with suflow.exe for Windows binary pipe handling

Copyright (c) 2025 ZAU
"""

import sys
import os
import subprocess
from pathlib import Path

from PyQt6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QTextEdit, QLineEdit, QPushButton, QLabel, QComboBox,
    QSplitter, QFrame, QFileDialog, QMessageBox, QGroupBox
)
from PyQt6.QtCore import Qt, QThread, pyqtSignal
from PyQt6.QtGui import QFont, QColor, QPalette, QTextCursor

# Find project root (look for bin/suflow.exe)
def find_project_root():
    """Find the project root by looking for bin/suflow.exe
    
    Works for both development and distribution:
    - Development: walks up from zau/src/pysu/gui/ to find project root
    - Distribution: walks up from dist/cwpsu-windows-x64/zau/src/pysu/gui/ to find distribution root
    """
    current = Path(__file__).resolve().parent
    for _ in range(15):  # Look up to 15 levels (increased for distribution support)
        if (current / "bin" / "suflow.exe").exists():
            return current
        current = current.parent
    # Fallback to environment variable or current directory
    return Path(os.environ.get("CWPROOT", "."))

PROJECT_ROOT = find_project_root()
SUFLOW_PATH = PROJECT_ROOT / "bin" / "suflow.exe"
DEFAULT_DATA = PROJECT_ROOT / "src" / "su" / "tutorial" / "data.su"

# Preset test workflows
TEST_WORKFLOWS = {
    "A - Basic Range Check": {
        "flow": "bin\\surange.exe",
        "description": "Display header ranges for input data",
        "input": str(DEFAULT_DATA)
    },
    "B - Get Headers": {
        "flow": "bin\\sugethw.exe key=tracl,ns,dt",
        "description": "Extract specific header values",
        "input": str(DEFAULT_DATA)
    },
    "C - AGC Gain + Range": {
        "flow": "bin\\sugain.exe agc=1 wagc=0.1 | bin\\surange.exe",
        "description": "Apply AGC and show resulting ranges",
        "input": str(DEFAULT_DATA)
    },
    "D - Normalize + Scale": {
        "flow": "bin\\sunormalize.exe | bin\\sugain.exe scale=0.5 | bin\\surange.exe",
        "description": "Normalize traces, scale, and show ranges",
        "input": str(DEFAULT_DATA)
    },
    "E - Gain Chain": {
        "flow": "bin\\sugain.exe tpow=2 | bin\\sugain.exe scale=0.1 | bin\\surange.exe",
        "description": "Apply time power correction, scale, show ranges",
        "input": str(DEFAULT_DATA)
    },
    "F - Header Manipulation": {
        "flow": "bin\\sushw.exe key=cdp a=100 | bin\\sugethw.exe key=cdp,tracl",
        "description": "Set CDP header and display results",
        "input": str(DEFAULT_DATA)
    }
}


class WorkerThread(QThread):
    """Thread for running suflow commands without blocking the GUI"""
    output_signal = pyqtSignal(str)
    finished_signal = pyqtSignal(int)
    
    def __init__(self, command, input_file=None, output_file=None):
        super().__init__()
        self.command = command
        self.input_file = input_file
        self.output_file = output_file
    
    def run(self):
        try:
            # Build suflow command
            args = [str(SUFLOW_PATH), self.command]
            if self.input_file:
                args.append(self.input_file)
            if self.output_file:
                args.append(self.output_file)
            
            self.output_signal.emit(f"$ suflow \"{self.command}\" {self.input_file or ''}\n")
            self.output_signal.emit("-" * 60 + "\n")
            
            # Run the process
            process = subprocess.Popen(
                args,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                cwd=str(PROJECT_ROOT)
            )
            
            # Stream output
            for line in process.stdout:
                self.output_signal.emit(line)
            
            process.wait()
            self.output_signal.emit("-" * 60 + "\n")
            self.output_signal.emit(f"Exit code: {process.returncode}\n\n")
            self.finished_signal.emit(process.returncode)
            
        except Exception as e:
            self.output_signal.emit(f"ERROR: {str(e)}\n")
            self.finished_signal.emit(1)


class SUFlowGUI(QMainWindow):
    """Main window for SU Flow GUI"""
    
    def __init__(self):
        super().__init__()
        self.worker = None
        self.init_ui()
    
    def init_ui(self):
        """Initialize the user interface"""
        self.setWindowTitle("SU Flow GUI - Seismic Unix Workflow Builder")
        self.setMinimumSize(900, 700)
        
        # Central widget
        central = QWidget()
        self.setCentralWidget(central)
        layout = QVBoxLayout(central)
        layout.setSpacing(10)
        layout.setContentsMargins(15, 15, 15, 15)
        
        # Title
        title = QLabel("SU Flow Builder")
        title.setStyleSheet("""
            QLabel {
                font-size: 24px;
                font-weight: bold;
                color: #00968f;
                padding: 5px;
            }
        """)
        layout.addWidget(title)
        
        # Preset workflows section
        preset_group = QGroupBox("Test Workflows")
        preset_group.setStyleSheet("""
            QGroupBox {
                font-weight: bold;
                border: 1px solid #444;
                border-radius: 5px;
                margin-top: 10px;
                padding-top: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
            }
        """)
        preset_layout = QHBoxLayout(preset_group)
        
        self.preset_combo = QComboBox()
        self.preset_combo.addItem("-- Select a test workflow --")
        for name in TEST_WORKFLOWS.keys():
            self.preset_combo.addItem(name)
        self.preset_combo.currentTextChanged.connect(self.on_preset_selected)
        self.preset_combo.setMinimumWidth(300)
        preset_layout.addWidget(self.preset_combo)
        
        self.preset_desc = QLabel("")
        self.preset_desc.setStyleSheet("color: #888; font-style: italic;")
        preset_layout.addWidget(self.preset_desc, 1)
        
        layout.addWidget(preset_group)
        
        # Input file section
        input_group = QGroupBox("Input Data")
        input_layout = QHBoxLayout(input_group)
        
        input_layout.addWidget(QLabel("Input File:"))
        self.input_file = QLineEdit()
        self.input_file.setPlaceholderText("Select or enter input .su file path")
        self.input_file.setText(str(DEFAULT_DATA))
        input_layout.addWidget(self.input_file, 1)
        
        browse_btn = QPushButton("Browse...")
        browse_btn.clicked.connect(self.browse_input)
        input_layout.addWidget(browse_btn)
        
        layout.addWidget(input_group)
        
        # Flow entry section
        flow_group = QGroupBox("SU Flow Pipeline")
        flow_layout = QVBoxLayout(flow_group)
        
        flow_hint = QLabel("Enter SU commands separated by | (pipes)")
        flow_hint.setStyleSheet("color: #888; font-size: 11px;")
        flow_layout.addWidget(flow_hint)
        
        self.flow_input = QLineEdit()
        self.flow_input.setPlaceholderText("e.g., sugain agc=1 | sufilter f=10,20,80,100 | sustack")
        self.flow_input.setStyleSheet("""
            QLineEdit {
                font-family: Consolas, 'Courier New', monospace;
                font-size: 14px;
                padding: 8px;
                border: 2px solid #444;
                border-radius: 4px;
                background: #1a1a1a;
            }
            QLineEdit:focus {
                border-color: #00968f;
            }
        """)
        self.flow_input.returnPressed.connect(self.run_flow)
        flow_layout.addWidget(self.flow_input)
        
        # Buttons row
        btn_layout = QHBoxLayout()
        
        self.run_btn = QPushButton("▶ Run Flow")
        self.run_btn.setStyleSheet("""
            QPushButton {
                background-color: #00968f;
                color: white;
                font-weight: bold;
                padding: 10px 25px;
                border-radius: 4px;
                font-size: 14px;
            }
            QPushButton:hover {
                background-color: #00b3a6;
            }
            QPushButton:pressed {
                background-color: #007a73;
            }
            QPushButton:disabled {
                background-color: #444;
                color: #888;
            }
        """)
        self.run_btn.clicked.connect(self.run_flow)
        btn_layout.addWidget(self.run_btn)
        
        clear_btn = QPushButton("Clear Console")
        clear_btn.setStyleSheet("""
            QPushButton {
                padding: 10px 20px;
                border-radius: 4px;
            }
        """)
        clear_btn.clicked.connect(self.clear_console)
        btn_layout.addWidget(clear_btn)
        
        btn_layout.addStretch()
        flow_layout.addLayout(btn_layout)
        
        layout.addWidget(flow_group)
        
        # Console output section
        console_group = QGroupBox("Console Output")
        console_layout = QVBoxLayout(console_group)
        
        self.console = QTextEdit()
        self.console.setReadOnly(True)
        self.console.setStyleSheet("""
            QTextEdit {
                font-family: Consolas, 'Courier New', monospace;
                font-size: 12px;
                background-color: #0d0d0d;
                color: #00ff00;
                border: 1px solid #333;
                border-radius: 4px;
                padding: 5px;
            }
        """)
        self.console.setMinimumHeight(300)
        console_layout.addWidget(self.console)
        
        layout.addWidget(console_group, 1)  # Stretch factor 1 to fill space
        
        # Status bar
        self.statusBar().showMessage(f"Project: {PROJECT_ROOT}")
        self.statusBar().setStyleSheet("color: #888;")
        
        # Initial console message
        self.console.append("SU Flow GUI initialized")
        self.console.append(f"suflow.exe: {SUFLOW_PATH}")
        self.console.append(f"Project root: {PROJECT_ROOT}")
        self.console.append("-" * 60)
        self.console.append("Select a test workflow or enter a custom flow pipeline.\n")
    
    def on_preset_selected(self, text):
        """Handle preset workflow selection"""
        if text in TEST_WORKFLOWS:
            preset = TEST_WORKFLOWS[text]
            self.flow_input.setText(preset["flow"])
            self.input_file.setText(preset["input"])
            self.preset_desc.setText(preset["description"])
        else:
            self.preset_desc.setText("")
    
    def browse_input(self):
        """Open file browser for input file"""
        filename, _ = QFileDialog.getOpenFileName(
            self,
            "Select Input SU File",
            str(PROJECT_ROOT),
            "SU Files (*.su);;All Files (*)"
        )
        if filename:
            self.input_file.setText(filename)
    
    def run_flow(self):
        """Execute the SU flow pipeline"""
        flow = self.flow_input.text().strip()
        input_file = self.input_file.text().strip()
        
        if not flow:
            QMessageBox.warning(self, "Error", "Please enter a flow pipeline")
            return
        
        if not input_file or not os.path.exists(input_file):
            QMessageBox.warning(self, "Error", f"Input file not found: {input_file}")
            return
        
        if not SUFLOW_PATH.exists():
            QMessageBox.warning(self, "Error", f"suflow.exe not found: {SUFLOW_PATH}")
            return
        
        # Disable run button while running
        self.run_btn.setEnabled(False)
        self.run_btn.setText("Running...")
        
        # Start worker thread
        self.worker = WorkerThread(flow, input_file)
        self.worker.output_signal.connect(self.append_output)
        self.worker.finished_signal.connect(self.on_flow_finished)
        self.worker.start()
    
    def append_output(self, text):
        """Append text to console output"""
        self.console.moveCursor(QTextCursor.MoveOperation.End)
        self.console.insertPlainText(text)
        self.console.moveCursor(QTextCursor.MoveOperation.End)
    
    def on_flow_finished(self, exit_code):
        """Handle flow completion"""
        self.run_btn.setEnabled(True)
        self.run_btn.setText("▶ Run Flow")
        
        if exit_code == 0:
            self.statusBar().showMessage("Flow completed successfully", 5000)
        else:
            self.statusBar().showMessage(f"Flow failed with exit code {exit_code}", 5000)
    
    def clear_console(self):
        """Clear the console output"""
        self.console.clear()


def apply_dark_theme(app):
    """Apply dark theme to the application"""
    palette = QPalette()
    
    # Dark background colors
    dark_bg = QColor(30, 30, 30)
    darker_bg = QColor(20, 20, 20)
    light_text = QColor(220, 220, 220)
    accent = QColor(0, 150, 136)  # Teal accent
    
    palette.setColor(QPalette.ColorRole.Window, dark_bg)
    palette.setColor(QPalette.ColorRole.WindowText, light_text)
    palette.setColor(QPalette.ColorRole.Base, darker_bg)
    palette.setColor(QPalette.ColorRole.AlternateBase, dark_bg)
    palette.setColor(QPalette.ColorRole.ToolTipBase, dark_bg)
    palette.setColor(QPalette.ColorRole.ToolTipText, light_text)
    palette.setColor(QPalette.ColorRole.Text, light_text)
    palette.setColor(QPalette.ColorRole.Button, dark_bg)
    palette.setColor(QPalette.ColorRole.ButtonText, light_text)
    palette.setColor(QPalette.ColorRole.BrightText, QColor(255, 0, 0))
    palette.setColor(QPalette.ColorRole.Link, accent)
    palette.setColor(QPalette.ColorRole.Highlight, accent)
    palette.setColor(QPalette.ColorRole.HighlightedText, QColor(0, 0, 0))
    
    app.setPalette(palette)


def main():
    """Main entry point"""
    app = QApplication(sys.argv)
    
    # Apply dark theme
    apply_dark_theme(app)
    app.setStyle("Fusion")
    
    # Additional stylesheet for global elements
    app.setStyleSheet("""
        QMainWindow {
            background-color: #1e1e1e;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #444;
            border-radius: 5px;
            margin-top: 10px;
            padding-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 5px;
            color: #00968f;
        }
        QComboBox {
            padding: 5px;
            border: 1px solid #444;
            border-radius: 3px;
            background: #2a2a2a;
        }
        QComboBox:hover {
            border-color: #00968f;
        }
        QComboBox::drop-down {
            border: none;
        }
        QComboBox QAbstractItemView {
            background: #2a2a2a;
            selection-background-color: #00968f;
        }
        QPushButton {
            background-color: #3a3a3a;
            border: 1px solid #555;
            padding: 5px 15px;
            border-radius: 3px;
        }
        QPushButton:hover {
            background-color: #4a4a4a;
            border-color: #00968f;
        }
        QLineEdit {
            padding: 5px;
            border: 1px solid #444;
            border-radius: 3px;
            background: #2a2a2a;
        }
        QLineEdit:focus {
            border-color: #00968f;
        }
        QScrollBar:vertical {
            background: #1a1a1a;
            width: 12px;
            border-radius: 6px;
        }
        QScrollBar::handle:vertical {
            background: #444;
            border-radius: 6px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover {
            background: #555;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    """)
    
    window = SUFlowGUI()
    window.show()
    
    sys.exit(app.exec())


if __name__ == "__main__":
    main()
