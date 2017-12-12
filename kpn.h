#ifndef KPN_H
#define KPN_H

#include <systemc.h>


class buffer : public sc_fifo<uint32_t>
{
public:
    uint32_t read()
    {
        uint32_t data;
        return data;
    }

    void write(uint32_t & data)
    {

        return;
    }

    buffer(uint32_t buffer_size){this->buffer_size = buffer_size;}

private:
    uint32_t buffer_size;

};

SC_MODULE(adder)
{
    public:
    sc_fifo<uint32_t> a_in;
    sc_fifo<uint32_t> b_out;
    sc_fifo<uint32_t> c_in;

    SC_CTOR(adder) : a_in("a_in", 10), b_out("b_out", 10), c_in("c_in", 10)
    {
        SC_THREAD(add);
    }

    private:
    void add(){return;}
};

SC_MODULE(spliter)
{
    public:
    sc_fifo<uint32_t> a_out;
    sc_fifo<uint32_t> b_in;
    sc_fifo<uint32_t> d_out;
    sc_out<uint32_t> e_out;

    SC_CTOR(spliter) : a_out("a_out", 10), b_in("b_in", 10), d_out("d_out", 10), e_out("e_out")
    {
        SC_THREAD(split);
    }

    private:
    void split(){return;}
};

SC_MODULE(delayer)
{
    public:
    sc_fifo<uint32_t> c_out;
    sc_fifo<uint32_t> d_in;

    SC_CTOR(delayer) : c_out("c_out", 10), d_in("d_in", 10)
    {
        SC_THREAD(delay);
    }

    private:
    void delay(){return;}
};


SC_MODULE(kpn)
{
    private:
    buffer a;
    buffer b;
    buffer c;
    buffer d;

    adder add;
    delayer delay;
    spliter split;

    public:
    SC_CTOR(kpn) : a(10), b(10), c(10), d(10), add("add"), delay("delay"), split("split")
    {

    }
};

#endif // KPN_H
