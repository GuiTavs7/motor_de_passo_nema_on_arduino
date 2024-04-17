# POJETO MOTOR DE PASSO NEMA
# AUTOR: GUILHERME TAVARES PINHEIRO
# DATA: 21/03/2024

# INICIO DO SCRIPT - CODANDO COM UTF-8 (PADRAO DE TECLADO)

# ----------------------------------------------------------------------------------------------

import subprocess # importa biblioteca de subprocessos
import time # importa bibliooteca de tempo
from pynput.keyboard import Controller, Key # importa bibilioteca de enviar atalhos via teclado

# ----------------------------------------------------------------------------------------------

# FUNCAO PARA ABRIR O ARDUINO IDE E O SERIAL MONITOR CONSECUTIVAMENTE

# ----------------------------------------------------------------------------------------------

def open_arduino_ide_with_serial_monitor(): 
    try:
        # Abre o Arduino IDE
        arduino_process = subprocess.Popen(['arduino'])
        
        # Aguarda um momento para o Arduino IDE abrir completamente
        time.sleep(45)
        
        # Envia os atalhos de teclado para abrir o Serial Monitor
        keyboard = Controller()
        keyboard.press(Key.ctrl_l)
        keyboard.press(Key.shift)
        keyboard.press('m')
        keyboard.release('m')
        keyboard.release(Key.shift)
        keyboard.release(Key.ctrl_l)
        
        print("MONITOR SERIAL ABERTO!")
        
        # Espera que o Arduino IDE seja fechado
        arduino_process.wait()
        
    # Caso o IDE falhe em abrir, este bloco abaixo eh executada!
    
    except FileNotFoundError:
        print("Erro: Verifique se o Arduino IDE esta instalado.")
        return
        
# FIM DA FUNCAO PARA ABRIR O SERIAL MONITOR DO ARDUINO IDE        

# ----------------------------------------------------------------------------------------------

# FUNCAO QUE ABRE O COMPILADOR "GEANY" DO RASPBIAN E O ARQUIVO ".ino"

def open_geany_with_arduino():
    try:
        # Abre o Geany com o arquivo do Arduino
        subprocess.Popen(['geany', '/path/to/your/arduino_sketch.ino'])
        
    except FileNotFoundError:
        print("Erro: Verifique se o Geany esta instalado e se o caminho para o seu arquivo Arduino esta correto.")
        return
        
# FIM DA FUNCAO

# ----------------------------------------------------------------------------------------------

# SE TUDO OCORRER BEM, EXECUTA AS DUAS FUNCOES ACIMA

if __name__ == "__main__":
    open_arduino_ide_with_serial_monitor()
    open_geany_with_arduino()
    
# ----------------------------------------------------------------------------------------------

# FIM DO SCRIPT
