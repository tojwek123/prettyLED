import sys
import serial
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

class MainWindow(QColorDialog):
	
	def __init__(self, parent=None):
		super().__init__(parent)
		self.currentColorChanged.connect(self.onCurrentColorChanged)
		self.serial = serial.Serial()
		self.serial.baudrate = 9600
		self.serial.port = 'COM4'
		self.serial.open()
		
	@pyqtSlot(QColor)
	def onCurrentColorChanged(self, color):
		writeStr = '{} {} {}\n'.format(color.red(), color.green(), color.blue()).encode()
		print(writeStr)
		self.serial.write(writeStr)
		
def main():
	app = QApplication(sys.argv)
	
	m = MyColorDialog()
	m.show()
	
	sys.exit(app.exec_())

if __name__ == '__main__':
	main()