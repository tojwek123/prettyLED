from PyQt5.QtWidgets import *
import sys

from mainwindow import MainWindow
from dialogs import SerialSettings

def main():
	app = QApplication(sys.argv)
	# mainWindow = MainWindow()
	# mainWindow.show()
	ss = SerialSettings()
	ss.show()
	sys.exit(app.exec_())

if __name__ == '__main__':
	main()