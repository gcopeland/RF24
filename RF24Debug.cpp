#include "RF24_config.h"
#include "RF24.h"

#if defined(__AVR_ATtinyX4__) || defined(__AVR_ATtinyX5__)
#  define FLASH_PTR(x) ((fstr_t *)x)
#else
#  define FLASH_PTR(x) ((const __FlashStringHelper *)x)
#endif

void RF24Debug::print_name(const char *name)
{
  _out.print(FLASH_PTR(name));
  if (strlen_P(name) < 8)
    _out.print('\t');
  _out.print(F("\t ="));
}

void RF24Debug::print_hex_byte(uint8_t b)
{
    _out.print(F(" 0x"));
    if (b < 16)
      _out.print('0');
    _out.print(b, HEX);
}

void RF24Debug::print_byte_register(const char *name, uint8_t reg, uint8_t qty)
{
  print_name(name);
  while (qty--)
    print_hex_byte(read_register(reg++));
  _out.println();
}

void RF24Debug::print_address_register(const char *name, uint8_t reg, uint8_t qty)
{
  print_name(name);

  while (qty--)
  {
    uint8_t buffer[5];
    read_register(reg++,buffer,sizeof buffer);

    _out.print(F(" 0x"));
    uint8_t* bufptr = buffer + sizeof buffer;
    while( --bufptr >= buffer ) {
      uint8_t b = *bufptr;
      if (b < 10)
        _out.print('0');
      _out.print(b, HEX);
    }
  }
  _out.println();
}

void RF24Debug::print_status(uint8_t status)
{
  print_name(PSTR("STATUS"));
  print_hex_byte(status);
  _out.print(F(" RX_DR="));
  _out.print((status & _BV(RX_DR))? 1: 0);
  _out.print(F(" TX_DS="));
  _out.print((status & _BV(TX_DS))? 1: 0);
  _out.print(F(" MAX_RT="));
  _out.print((status & _BV(MAX_RT))? 1: 0);
  _out.print(F(" RX_P_NO="));
  _out.print((status >> RX_P_NO) & B111);
  _out.print(F(" TX_FULL="));
  _out.println((status & _BV(TX_FULL))? 1: 0);
}

static const char rf24_datarate_e_str_0[] PROGMEM = "1MBPS";
static const char rf24_datarate_e_str_1[] PROGMEM = "2MBPS";
static const char rf24_datarate_e_str_2[] PROGMEM = "250KBPS";
static const char * const rf24_datarate_e_str_P[] PROGMEM = {
  rf24_datarate_e_str_0,
  rf24_datarate_e_str_1,
  rf24_datarate_e_str_2,
};
static const char rf24_model_e_str_0[] PROGMEM = "nRF24L01";
static const char rf24_model_e_str_1[] PROGMEM = "nRF24L01+";
static const char * const rf24_model_e_str_P[] PROGMEM = {
  rf24_model_e_str_0,
  rf24_model_e_str_1,
};
static const char rf24_crclength_e_str_0[] PROGMEM = "Disabled";
static const char rf24_crclength_e_str_1[] PROGMEM = "8 bits";
static const char rf24_crclength_e_str_2[] PROGMEM = "16 bits" ;
static const char * const rf24_crclength_e_str_P[] PROGMEM = {
  rf24_crclength_e_str_0,
  rf24_crclength_e_str_1,
  rf24_crclength_e_str_2,
};
static const char rf24_pa_dbm_e_str_0[] PROGMEM = "PA_MIN";
static const char rf24_pa_dbm_e_str_1[] PROGMEM = "PA_LOW";
static const char rf24_pa_dbm_e_str_2[] PROGMEM = "PA_HIGH";
static const char rf24_pa_dbm_e_str_3[] PROGMEM = "PA_MAX";
static const char * const rf24_pa_dbm_e_str_P[] PROGMEM = { 
  rf24_pa_dbm_e_str_0,
  rf24_pa_dbm_e_str_1,
  rf24_pa_dbm_e_str_2,
  rf24_pa_dbm_e_str_3,
};

void RF24Debug::printDetails(void)
{
  print_status(get_status());

  print_address_register(PSTR("RX_ADDR_P0-1"),RX_ADDR_P0,2);
  print_byte_register(PSTR("RX_ADDR_P2-5"),RX_ADDR_P2,4);
  print_address_register(PSTR("TX_ADDR"),TX_ADDR);

  print_byte_register(PSTR("RX_PW_P0-6"),RX_PW_P0,6);
  print_byte_register(PSTR("EN_AA"),EN_AA);
  print_byte_register(PSTR("EN_RXADDR"),EN_RXADDR);
  print_byte_register(PSTR("RF_CH"),RF_CH);
  print_byte_register(PSTR("RF_SETUP"),RF_SETUP);
  print_byte_register(PSTR("CONFIG"),CONFIG);
  print_byte_register(PSTR("DYNPD/FEATURE"),DYNPD,2);

#if defined(ESP8266)
  _out.print(F("Data Rate\t = "));
  _out.println(FLASH_PTR(pgm_read_dword(&rf24_datarate_e_str_P[getDataRate()])));
  _out.print(F("Model\t\t = "));
  _out.println(FLASH_PTR(pgm_read_dword(&rf24_model_e_str_P[isPVariant()])));
  _out.print(F("CRC Length\t = "));
  _out.println(FLASH_PTR(pgm_read_dword(&rf24_crclength_e_str_P[getCRCLength()])));
  _out.print(F("PA Power\t = "));
  _out.println(FLASH_PTR(pgm_read_dword(&rf24_pa_dbm_e_str_P[getPALevel()])));
#else
  _out.print(F("Data Rate\t = "));
  _out.println(FLASH_PTR(pgm_read_word(&rf24_datarate_e_str_P[getDataRate()])));
  _out.print(F("Model\t\t = "));
  _out.println(FLASH_PTR(pgm_read_word(&rf24_model_e_str_P[isPVariant()])));
  _out.print(F("CRC Length\t = "));
  _out.println(FLASH_PTR(pgm_read_word(&rf24_crclength_e_str_P[getCRCLength()])));
  _out.print(F("PA Power\t = "));
  _out.println(FLASH_PTR(pgm_read_word(&rf24_pa_dbm_e_str_P[getPALevel()])));
#endif
}

void RF24Debug::on_write_register(uint8_t reg, uint8_t value)
{
  _out.print(F("write_register("));
  print_hex_byte(reg);
  _out.print(',');
  print_hex_byte(value);
  _out.println(')');
}

void RF24Debug::observe_tx(uint8_t value)
{
  _out.print(F("OBSERVE_TX"));
  print_hex_byte(value);
  _out.print(F(": PLOS_CNT="));
  _out.print((value >> PLOS_CNT) & B1111, HEX);
  _out.print(F(" ARC_CNT="));
  _out.println((value >> ARC_CNT) & B1111, HEX);
}

void RF24Debug::on_status(uint8_t status)
{
  print_status(status);
  _out.println(status & _BV(TX_DS)? F("...OK."): F("...Failed"));
}

void RF24Debug::on_ack(uint8_t ack_len)
{
  _out.print(F("got ack "));
  _out.println(ack_len);
}

void RF24Debug::on_write_payload(uint8_t data_len, uint8_t blank_len)
{
  _out.print(F("Writing "));
  _out.print(data_len, DEC);
  _out.print(F(" bytes "));
  _out.println(blank_len, DEC);
}

void RF24Debug::on_read_payload(uint8_t data_len, uint8_t blank_len)
{
  _out.print(F("Reading "));
  _out.print(data_len);
  _out.print(F(" bytes "));
  _out.println(blank_len);
}
