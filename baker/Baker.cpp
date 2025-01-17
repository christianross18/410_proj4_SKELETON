#include <mutex>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;
//ID is just a number used to identify this particular baker
//(used with PRINT statements)
Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}
//bake, box and append to anOrder.boxes vector
//if order has 13 donuts there should be 2 boxes
//1 with 12 donuts, 1 with 1 donut
void Baker::bake_and_box(ORDER &anOrder) {
	int originial = anOrder.number_donuts;

	while(anOrder.number_donuts>0){
		Box b;
		DONUT d;
		while(anOrder.number_donuts>0 && b.addDonut(d)){
			anOrder.number_donuts--;
		}
		anOrder.boxes.push_back(b);
	}
	anOrder.number_donuts=originial;
}
//as long as there are orders in order_in_Q then
//for each order:
//	create box(es) filled with number of donuts in the order
//  then place finished order on order_outvector
//  if waiter is finished (b_WaiterIsFinished) then
//  finish up remaining orders in order_in_Q and exit
//
//You will use cv_order_inQ to be notified by waiter
//when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
//hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
void Baker::beBaker() {
	ORDER a;

	while(true){
		{
			unique_lock<mutex> lck(mutex_order_inQ);
			while(order_in_Q.empty() && !b_WaiterIsFinished){
				cv_order_inQ.wait(lck);
			}
			if(order_in_Q.empty() && b_WaiterIsFinished){
				break;
			}
			a=order_in_Q.front();
			order_in_Q.pop();
		}
		bake_and_box(a);
		{
			lock_guard<mutex> lck(mutex_order_outQ);
			order_out_Vector.push_back(a);
		}
	}
	
}
