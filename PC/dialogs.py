from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import serial.tools.list_ports as listPorts

class SerialSettings(QDialog):
	
	_RefreshPortsItem = '<refresh list>'
	
	def __init__(self, parent=None):
		super().__init__(parent)
		
		self._baudLabel = QLabel('Baud rate:', self)
		self._baudSpinBox = QSpinBox(self)
		self._portLabel = QLabel('Port:', self)
		self._portComboBox = QComboBox(self)
		self._portComboBox.activated.connect(self._onPortComboBoxActivated)
		self._refreshPortComboBox()
		
		self.layout = QGridLayout(self)
		self.layout.addWidget(self._baudLabel, 0, 0)
		self.layout.addWidget(self._baudSpinBox, 0, 1)
		self.layout.addWidget(self._portLabel, 1, 0)
		self.layout.addWidget(self._portComboBox, 1, 1)
		self.setLayout(self.layout)
		
	def _refreshPortComboBox(self):
		ports = self._getAvailablePorts()
		self._portComboBox.clear()
		self._portComboBox.addItems(ports)
		self._portComboBox.addItem(self._RefreshPortsItem)
	
	@pyqtSlot(int)
	def _onPortComboBoxActivated(self, index):
		if self._portComboBox.itemText(index) == self._RefreshPortsItem:
			self._refreshPortComboBox()
		
	@staticmethod
	def _getAvailablePorts():
		ports = listPorts.comports()
		
		#TEST:
		ports = ['COM1', 'COM2', 'COM3']
		
		return ports