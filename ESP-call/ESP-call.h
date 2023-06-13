// ESP-call.h

// ======================================================================

typedef struct NodeMCU_PGM_STRUCT {
    const char *code;               // NodeMCU code
    int delay;                      // delay time in 1/10 sec unit
} NodeMCU_PGM;

// ======================================================================


extern RawSerial PC;                // PC = (USBTX, USBRX)
extern RawSerial ESP;               // ESP = (D1=TX, D0=RX)

// ======================================================================

extern char ESP_recv_buffer[];      // ESP receive buffer
extern int ESP_recv_buffer_index;   // ESP receive buffer index

// ======================================================================

void ISR_PC_to_ESP();
void ISR_ESP_to_PC();

void ISR_ESP_to_recv_buffer();

// ======================================================================

void ESP_reset(void);

void ESP_echo(void);
void ESP_noecho(void);

char *ESP_call_single(NodeMCU_PGM pgm);
void ESP_call_multi(NodeMCU_PGM pgms[]);

// ======================================================================