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

__3.Récupération de l'étalonnage__

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
Avec cette fonction on récupère les données d'étalonnage du capteur BMP280. Ces données vont nous permettre de calculer la température et la pression compensées. On reçoit les 26 octets de l'étalonnage. Ceci n'est utile qu'ici car pas tout les octets ne servent pour calculer la température ou la pression.<br/>

__4.Récupération de la température compensée__

Voici ci-dessous les deux fonctions nous permettant d'obtenir une valeur de température compensée : 

```c
float BMP280_readRawTemp(){
	uint8_t temp_msb_register = (uint8_t)TEMP_MSB;
	uint8_t temp_frame_rx[3];

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &temp_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, temp_frame_rx, 3, HAL_MAX_DELAY)){

		}
	}

	uint32_t rawTemp = (temp_frame_rx[0]<<12) + (temp_frame_rx[1]<<4) + (temp_frame_rx[2]>>4);

	uint8_t tempArray[6] = {0};
	temp_msb_register = (uint8_t)CALIB_TEMP_START;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &temp_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, tempArray, 6, HAL_MAX_DELAY)){

		}
	}

	return (float)BMP280_compensateTemp(tempArray, rawTemp)/100;
}
```
La fonction ci-dessous est fournie dans la documentation du capteur BMP280.
```c
uint32_t BMP280_compensateTemp(uint8_t *calib, uint32_t rawTemp){
	uint32_t dig_T1 = calib[0] | calib[1]<<8;
	uint32_t dig_T2 = calib[2] | calib[3]<<8;
	uint32_t dig_T3	= calib[4] | calib[5]<<8;

	uint32_t var1 = ((((rawTemp >> 3) - (dig_T1 << 1)))
			* dig_T2) >> 11;
	uint32_t var2 = (((((rawTemp >> 4) - dig_T1)
			* ((rawTemp >> 4) - dig_T1)) >> 12)
			* dig_T3) >> 14;

	uint32_t t_fine = var1 + var2;

	return (t_fine * 5 + 128) >> 8;
}
```

__5.Récupération de la pression compensée__

Même principe pour obtenir la pression compensée : 

```c
float BMP280_readRawPress(){
	uint8_t press_msb_register = (uint8_t)PRESS_MSB;
	uint8_t press_frame_rx[3];

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &press_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, press_frame_rx, 3, HAL_MAX_DELAY)){

		}
	}

	uint32_t rawPress = (press_frame_rx[0]<<12) + (press_frame_rx[1]<<4) + (press_frame_rx[2]>>4);

	uint8_t pressArray[18] = {0};
	press_msb_register = (uint8_t)CALIB_PRESS_START;

	if(HAL_OK == HAL_I2C_Master_Transmit(&hi2c1, BMP_I2C_ADD, &press_msb_register, 1, HAL_MAX_DELAY)){
		if(HAL_OK == HAL_I2C_Master_Receive(&hi2c1, BMP_I2C_ADD, pressArray, 6, HAL_MAX_DELAY)){

		}
	}


	return BMP280_compensatePress(pressArray, rawPress)/256;
}
```

```c
uint32_t BMP280_compensatePress(uint8_t *calib, uint32_t rawPress) {
	uint32_t dig_P1 = calib[0] | calib[1]<<8;
	uint32_t dig_P2 = calib[2] | calib[3]<<8;
	uint32_t dig_P3	= calib[4] | calib[5]<<8;
	uint32_t dig_P4	= calib[6] | calib[7]<<8;
	uint32_t dig_P5	= calib[8] | calib[9]<<8;
	uint32_t dig_P6	= calib[10] | calib[11]<<8;
	uint32_t dig_P7	= calib[12] | calib[13]<<8;
	uint32_t dig_P8	= calib[14] | calib[15]<<8;
	uint32_t dig_P9	= calib[16] | calib[17]<<8;

	uint32_t t_fine = 0;
	uint64_t var1, var2, p;

	var1 = ((uint64_t)t_fine) - 128000;
	var2 = var1 * var1 * (uint64_t)dig_P6;
	var2 = var2 + ((var1*(uint64_t)dig_P5)<<17);
	var2 = var2 + (((uint64_t)dig_P4)<<35);
	var1 = ((var1 * var1 * (uint64_t)dig_P3)>>8) + ((var1 * (uint64_t)dig_P2)<<12);
	var1 = (((((uint64_t)1)<<47)+var1))*((uint64_t)dig_P1)>>33;
	if(var1 == 0) {
		return 0;
	}
	p = 1048576-rawPress;
	p = (((p<<31)-var2)*3125)/var1;
	var1 = (((uint64_t)dig_P9) * (p>>13) * (p>>13)) >> 25;
	var2 = (((uint64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((uint64_t)dig_P7)<<4);
	return (uint32_t)p;
}
```

## TP3 - Interface REST

A l'aide de la bibiothèque Flask, nous avons développé notre serveur web sur la Raspberry Pi 0.
Ce serveur doit faire ses réponses au format JSON et traiter les différentes méthodes HTTP

Durant ce TP, nous avons travaillé sur la chaîne de caractères suivante : 
```python
welcome = "Welcome to 3ESE API!"
```
### API CRUD

Nous avons implémenté l'API CRUD suivante : 

| __CRUD__ | __Method__ | __Path__ | __Action__ |
| --- | --- | --- | --- |
| Create | POST | welcome/ | Change sentence |
| Retrieve | GET | welcome/ | Return sentence |
| Retrieve | GET | welcome/x | Return letter x |
| Update | PUT | welcome/x | Insert new word at position x |
| Update | PATCH | welcome/x | Change letter at position x |
| Delete | DELETE | welcome/x | Delete letter at position x |
| Delete | DELETE | welcome/ | Delete sentence |

```python
@app.route('/api/welcome/', methods=['GET'])
def api_welcome():
        return welcome
```
On retourne ici la chaîne de caractères en totalité.

```python
@app.route('/api/welcome/<int:index>', methods=['GET'])
def api_welcome_index(index):
        if index > len(welcome):
                abort(404)
        else:
                return jsonify({"index": index, "val": welcome[index]})
```

Cette fonction nous permet de retourner la lettre à la position index du mot welcome sous le format JSON. Si cet index est supérieur à la longueur du mot, on renvoie sur une page signalant une erreur 404.

```python
@app.route('/api/welcome/<int:index>', methods=['PATCH'])
def api_welcome_patch_index(index):
        global welcome
        data = request.get_json()
        if index >len(welcome) or len(data) != 1 :
                abort(404)
        else :
                temp = list(welcome)
                temp[index] = data
                welcome = "".join(temp)
                return welcome + "\r\n"
```
Cette fonction a pour but de remplacer un caractère de la chaîne welcome à la position index. Si l'index est supérieur à la longueur du mot ou que la data n'est pas une lettre, on renvoie sur une erreur 404. Si ce n'est pas le cas, on transforme la chaîne welcome en liste afin de lui affecter data en position index. Enfin, on utilise "".join(temp) pour avoir notre welcome modifié.

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
  
 
