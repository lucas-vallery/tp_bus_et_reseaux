# Tp bus et réseaux – Castellani Vallery

## Table des matières

- [Introduction](#introduction)
- [TP1 - Bus I2C](#TP1---bus-i2c)
- [TP2 - Interfaçage STM32 - Raspberry](#TP2---interfacage-STM32---raspberry)
- [TP3 - Interface REST](#TP3---interface-REST)
- [TP4 - Bus CAN](#TP4---Bus-CAN)
- [TP5 - Intégration I²C - Serial - REST - CAN](#TP5---integration-i2c---Serial---REST---CAN)
- [Conclusion](#conclusion)

## Introduction

Ces différents TPs nous ont permis de réaliser : 
- Une liaison I²C entre la STM32 Nucleo et un capteur de pression et de température
- Une liaison UART entre la STM32 et un Raspberry Pi
- Une interface Web sur le Raspberry Pi
- Une liaison CAN entre la STM32 et une carte pilotant un moteur pas à pas

<img src="/Users/jean-francoiscastellani/Downloads/path1315-0-9-37.png" width="80%" height="80%">

## TP1 - Bus I2C

Dans ce premier TP nous cherchons à mettre en place une communication I²C entre entre le microcontrôleur et deux composants I²C. Le microncontrôleur STM32 joue le rôle de maître sur le bus.

### Capteur BMP280
La capteur BMP280 est un capteur de température est de pression. Ces deux grandeurs sont mesurées par deux composants I²C distincts.

### Librairie pour le BMP280

On a divisé cette librairie en deux fichiers : BMP280.c et BMP280.h
Le premier fichier est celui dans lequel nous avons écrit notre code nécessaire à la communication I²C tandis que dans le second nous avons placé l'adresse I²C du capteur, les valeurs des registres et les prototypes des fonctions.

__1. Identification du BMP280__

Le principe de cette fonction est d'envoyer l'adresse du registre ID et de recevoir un octet contenant l'identifiant du capteur.

```c
uint8_t BMP280_getId(){
	uint8_t id_register = (uint8_t)ID;
	uint8_t id_rx_value;
	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &id_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, &id_rx_value, 1, HAL_MAX_DELAY)){

		}
	}
	return id_rx_value;
}
```
__2. Configuration du BMP280__



```c
uint8_t BMP280_config(){
	uint8_t ctrl_meas_register = (uint8_t)CTRL_MEAS;
	uint8_t ctrl_meas_init_value = 0b01010111;

	uint8_t tx_frame[2] = {ctrl_meas_register, ctrl_meas_init_value};
	uint8_t ctrl_meas_rx_value;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, tx_frame, 2, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &ctrl_meas_register, 1, HAL_MAX_DELAY)){
			if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, &ctrl_meas_rx_value, 1, HAL_MAX_DELAY)){

			}
		}
	}
	return ctrl_meas_rx_value;
}
```
Pour configurer le BMP280, on envoie deux octets. Le premier contenant l'adresse du registre à modifier ici CTRL_MEAS et le second la valeur que l'on veut écrire. On créé un tableau de 2 éléments prenant ces deux paramètres et on les envoie à l'aide de la fonction HAL.<br/>
On a de plus ajouté la possibilité pour l'utilisateur de contrôler si la valeur a bien été écrite en la retournant.

__3.Récupération de l'étalonnage, de la température et de la pression__

```c
uint8_t BMP280_Etalonnage(uint8_t* calibration){
	uint8_t etalonnage_register = (uint8_t)CALIB_TEMP_START;
	for (int i = 0; i<26; i++){
		if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &etalonnage_register, 1, HAL_MAX_DELAY)){
			if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, &calibration, 26, HAL_MAX_DELAY)){

			}
		}
	}
	return 0;
}
```


#TP3

### Réponse d'une requette GET
```
 "args": {},
  "headers": {
    "Accept": "*/*",
    "Host": "192.168.88.239:5000",
    "User-Agent": "curl/7.74.0"
  },
  "method": "GET",
  "path": null,
  "url": "http://192.168.88.239:5000/api/request"
```

### Réponse d'une requette post 
```
"POST": {
    "data": {
      "Name": "Lucas"
    }
  },
  "args": {},
  "headers": {
    "Accept": "*/*",
    "Content-Length": "18",
    "Content-Type": "application/json",
    "Host": "192.168.88.239:5000",
    "User-Agent": "curl/7.74.0"
  },
  "method": "POST",
  "path": null,
  "url": "http://192.168.88.239:5000/api/request"
  ```
  
 
