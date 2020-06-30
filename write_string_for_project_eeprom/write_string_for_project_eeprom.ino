#define EEPROM_ID 0xA0

const int sda = 6;
const int scl = 7;
const int wp = 5;

//functions
void I2C_clock();
void I2C_start();
void I2C_stop();
void I2C_write(unsigned char);
byte I2C_read();
void I2C_Ack();
void I2C_NoAck();
void EEPROM_WriteByte(unsigned char, unsigned char);
unsigned char EEPROM_ReadByte(unsigned char);
void write_string(unsigned char*, unsigned char);
unsigned char read_string(unsigned char);
void read_eeprom_page(unsigned char);

void setup() {
  pinMode(sda, OUTPUT);
  pinMode(scl, OUTPUT);
  pinMode(wp, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  // Address bytes 
  byte password_match = 0x10;
  byte wrong_password = 0x20;
  byte old_password   = 0x30;
  byte new_password   = 0x40;
  byte go_to_home     = 0x50;
  byte conf_password  = 0x60;
  byte done           = 0x70;
  byte press_home     = 0x80;
  byte change_pass    = 0x90;
  byte press_enter    = 0xA0;
  byte enter_password = 0xB0;
  byte arrow          = 0xC0;
  byte incorrect      = 0xD0;
  byte password       = 0xE0;
  byte buzzer         = 0xC6;

  //for(unsigned char add = 0x00; add < 0xFF; add++) EEPROM_WriteByte(add, '\0'); //for cleaning the entire memory
  /*
  write_string("-Password Match-", password_match);
  write_string("-Wrong Password-", wrong_password);
  write_string("Old Password ?", old_password);
  write_string("New Password:", new_password);
  write_string("Go to Home", go_to_home);
  write_string("Conf. Password", conf_password);
  write_string("Done !!", done);
  write_string("Press Home", press_home);
  write_string("Change Password", change_pass);
  write_string("Press Enter..", press_enter);
  write_string("-Enter Password-", enter_password);
  write_string(":-> ", arrow);
  write_string("Incorrect", incorrect);
  write_string("12345", password);
  EEPROM_WriteByte(buzzer, 0);
  
  Serial.println("Done Writing EEPROM\n"); */

  unsigned char address = 0x10;
  unsigned char temp, temp_address;
  
  for(unsigned int i=0; i<16 && address <= 0xE0; i++) {
    temp_address = address;
    for(unsigned int j=0; j<16; j++) {
      temp = EEPROM_ReadByte(temp_address);
      if(temp == '\0') break;
      Serial.print((char)temp);
      temp_address++;
    }Serial.print('\n');
    address = address + 0x10;
  }
  Serial.println(EEPROM_ReadByte(buzzer)); // reading buzzer byte, coz its not a string and not present in a separate page*/
  

  /*
  unsigned char temp_address = password;
  for(unsigned int i=0; i<16; i++) {
    unsigned char temp = EEPROM_ReadByte(temp_address);
    Serial.print((char)temp);
    temp_address++;
  } Serial.print('\n'); */
  
  //read_eeprom_page(incorrect);
  //read_eeprom_page(password);
  //read_eeprom_page(password_match);
  
  while(1); //Precaution !!!!! This While is very much required
}

void I2C_clock() {
  delayMicroseconds(2);
  digitalWrite(scl, HIGH);
  delayMicroseconds(2);
  digitalWrite(scl, LOW);
}

void I2C_start() {
  digitalWrite(scl, LOW);
  digitalWrite(sda, HIGH);
  delayMicroseconds(2);
  digitalWrite(scl, HIGH);
  delayMicroseconds(2);
  digitalWrite(sda, LOW);
  delayMicroseconds(2);
  digitalWrite(scl, LOW); 
}

void I2C_stop() {
  digitalWrite(scl, LOW);
  delayMicroseconds(2);
  digitalWrite(sda, LOW);
  delayMicroseconds(2);
  digitalWrite(scl, HIGH);
  delayMicroseconds(2);
  digitalWrite(sda, HIGH);
}

void I2C_write(byte dat){
  digitalWrite(wp, LOW);
  bool temp;
  for(unsigned int i=0; i<8; i++) {
    temp = dat/B10000000;
    //Serial.print(temp);
    digitalWrite(sda, temp);
    I2C_clock();
    dat = dat<<1;
  } //Serial.print('\n');
  digitalWrite(sda, HIGH);
}

byte I2C_Read()
{
  digitalWrite(wp, HIGH);
  byte dat = 0x00;
  bool x;
  pinMode(sda, INPUT);

  //Serial.print("Read Function- ");
  for(unsigned int i=0; i<8; i++){
    delayMicroseconds(2);
    digitalWrite(scl, HIGH);
    delayMicroseconds(2);
    dat = dat<<1;
    x = digitalRead(sda); //Serial.print(x);
    dat = dat | x;
    digitalWrite(scl, LOW);
  }// Serial.print('\n');
  pinMode(sda, OUTPUT);
  return dat;
}

void I2C_Ack()
{
  digitalWrite(sda, LOW);
  I2C_clock();
  digitalWrite(sda, HIGH);
}

void I2C_NoAck()
{
  digitalWrite(sda, HIGH);
  I2C_clock();
  digitalWrite(scl, HIGH);
}

void EEPROM_WriteByte(unsigned char eeprom_Address, unsigned char eeprom_Data)
{
   I2C_start();               // Start i2c communication
     I2C_write(EEPROM_ID);     // connect to AT2404 by sending its ID on I2c Bus
  I2C_Ack();
    I2C_write(eeprom_Address); // Select the Specified EEPROM address of AT2404
  I2C_Ack();
    I2C_write(eeprom_Data);    // Write the data at specified address
  I2C_Ack();
    I2C_stop();                // Stop i2c communication after Writing the data
  delay(5);               // Write operation takes max 5ms, refer At2404 datasheet 
}

unsigned char EEPROM_ReadByte(unsigned char eeprom_Address)
{
  unsigned char eeprom_Data;

    I2C_start();               // Start i2c communication
     I2C_write(EEPROM_ID);     // connect to AT2404(write) by sending its ID on I2c Bus
  I2C_Ack();
    I2C_write(eeprom_Address); // Select the Specified EEPROM address of AT2404
    I2C_Ack();

    I2C_start();           // Start i2c communication
    I2C_write(0xA1);           // connect to AT2404(read) by sending its ID on I2c Bus
    I2C_Ack();
  eeprom_Data = I2C_Read();  // Read the data from specified address
  I2C_NoAck();
    I2C_stop();              // Stop i2c communication after Reading the data
  delay(5);
  return eeprom_Data;          // Return the Read data

}

void write_string(unsigned char* string_ptr, unsigned char eeprom_address){
  unsigned char temp;
  unsigned int i = 0;
  do {
    temp = string_ptr[i];
    EEPROM_WriteByte(eeprom_address, temp);
    eeprom_address++; i++;
  } while(temp!='\0');
  Serial.println("Done Writing!");
}

void read_eeprom_page(unsigned char page_address) {
  unsigned char temp_address = page_address;
  for(unsigned int i=0; i<16; i++) {
    unsigned char temp = EEPROM_ReadByte(temp_address);
    Serial.print((char)temp);
    temp_address++;
  } Serial.print('\n');
  
}
/*
 * unsigned char temp_address = password;
  for(unsigned int i=0; i<16; i++) {
    EEPROM_WriteByte(temp_address, '\0');
    temp_address++;
  }
  EEPROM_WriteByte(password, 'N');
  EEPROM_WriteByte(password+0x0F, 'H');
 */



 
