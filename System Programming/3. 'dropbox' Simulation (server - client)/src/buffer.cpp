#include "buffer.hpp"

void printBuffer(const bufferCirc *buff){
	bufferNode *buffNode = (bufferNode *)buff->buffer;
	for(int i = 0; i < buff->bufferSize; i++){
		if(buffNode->ipAdd==0){
			printf("(null)\n");
		}else{
			printf("(%lu, %u, %s, %d)\n", buffNode->ipAdd, buffNode->port, buffNode->pathname,buffNode->version);
		}
		buffNode++;
	}
}


void initializeBuffer(bufferCirc *buff, int buffSize){
	buff->buffer = malloc(sizeof(bufferNode)*buffSize);
		if(buff->buffer == NULL) { perror("malloc"); exit(1); }

	buff->bufferSize = buffSize;
	buff->sum = 0;
	buff->head = buff->buffer;
	buff->tail = buff->buffer;
}

void bufferAdd(bufferCirc *buff, uint32_t value1, uint16_t value2){
	bufferNode element;
	element.ipAdd		= value1;
	element.port		= value2;
	memset( element.pathname, 0, NULL );
	element.version		= -1;

	if(buff->sum==buff->bufferSize){ printf("full\n"); return ; }
	memcpy(buff->head, &element, sizeof(bufferNode));

	buff->head = (char *)buff->head + sizeof(bufferNode);
	if(buff->head == buff->endOfBuffer)
		buff->head = buff->buffer;
	buff->sum++;
}

void bufferAdd(bufferCirc *buff, bufferNode *element){
	if(buff->sum==buff->bufferSize){ printf("full\n"); return ; }

	memcpy(buff->head, element, sizeof(bufferNode));
	bufferNode *buffNode = (bufferNode *)buff->head;
	buff->head = (char *)buff->head + sizeof(bufferNode);
	if(buff->head == buff->endOfBuffer)
		buff->head = buff->buffer;
	buff->sum++;
}



void getBufferObject(bufferCirc *buff, bufferNode *element){
	if(buff->sum==0){ printf("empty\n"); return ; }

	memcpy(element, buff->tail, sizeof(bufferNode));
	buff->tail = (char *)buff->tail + sizeof(bufferNode);
	if(buff->tail == buff->endOfBuffer)
		buff->tail = buff->buffer;
	buff->sum--;
}




