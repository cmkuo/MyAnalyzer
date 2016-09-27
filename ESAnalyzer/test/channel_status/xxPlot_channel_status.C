#include "xxPlot_channel_status.h"

void xxPlot_channel_status(){
	ES_store ESdead;
	//input the dead
	ESdead.add_fed(548);
	ESdead.add_fed(549);
	ESdead.add_fed(555);
	ESdead.add_fed(556);
	//ESdead.add_sensor(z,p,x,y)
	ESdead.add_sensor(1,1,24,33);
	ESdead.add_sensor(1,1,25,33);
	xPlot_channel_status(ESdead,"20160000");
}