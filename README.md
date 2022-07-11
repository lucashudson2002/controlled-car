<h1 align="center"> Controlled Car </h1>
<p  align="center"> 
	This is a cart controlled by radio (NRF24L01), by bluetooth (HC-06) and by infrared, made with arduino. <br>
	:construction:  Project under construction  :construction:
</p>

<h2 align="center">  🔗 Resumo do projeto </h2>

- Será utilizado o motor dual-driver TB6612FNG para controlar os 4 motores.
- Para controlar a ponte-H TB6612FNG, utilizarei a biblioteca que eu mesmo criei: <a href="https://github.com/lucashudson2002/TB6612FNG" target="_blank">github.com/lucashudson2002/TB6612FNG</a>.
- Carro controlado através de 3 métodos diferentes (rádio, bluetooth, infravermelho).
- Possui um senhor de distância na frente (HC-SR04) para impedir que o carro bata com algum obstáculo, ou para evitar obstáculo caso esteja no modo de controle autônomo.
- O regulador buck step down vai fornecer +6V ao motores através da ponte-H, a bateria vai direto alimentar o arduino.
- Existe um limite mínimo de tensão da bateria para que o robô funcione normalmente, abaixo desse nível será sinalizado mau funcionamento.
- Possui um buzzer para emitir sinais sonoros, como de buzina, de aviso de baixa bateria, ou de aviso de obstáculo.
- Para ligar/desligar o carro haverá um interruptor para esse funcionamento.

<h2 align="center">  📁 Descrição dos arquivos </h2>

- Controlled-Car-main.ino -> arquivo que fica o programa principal
- diagrama.fzz -> arquivo que fica todas as ligações do projeto

<h2 align="center">  🛠️ Lista de itens </h2>

- 1 placa de acrílico branca ?cmX?cm
- 1 placa de fenolite ?cmX?cm
- Arduino nano
- 4 motores + 4 rodas
- 1 ponte-H TB6612FNG
- 1 NRF24L01
- 1 HC-05
- 1 Receptor IR + controle IR
- 1 HC-SR04 + suporte
- 1 servo motor
- 3 baterias de lítio de 3.7V + suporte
- 1 chave liga/desliga
- 1 regulador de tensão buck step down LM2596
- 1 buzzer
- Divisor de tensão e led vermelho para medir/sinalizar nível de bateria
- Capacitores, resistores, diodo, parafusos, porcas, bornes KRE 2 vias, barras de pinos fêmea e macho, fios, solda.

<h2 align="center">  ✔️ Técnicas e tecnologias utilizadas </h2>

- ``Arduino``
- ``C++``
- ``POO``
