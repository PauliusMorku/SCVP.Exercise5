#ifndef KPN_H
#define KPN_H

#include <systemc.h>

SC_MODULE(adder)
{
    public:
    sc_fifo_in<uint32_t> fifo_a_in;
    sc_fifo_out<uint32_t> fifo_b_out;
    sc_fifo_in<uint32_t> fifo_c_in;

    SC_CTOR(adder) : fifo_a_in("fifo_a_in"), fifo_b_out("fifo_b_out"), fifo_c_in("fifo_c_in")
    {
        SC_THREAD(add);
    }

    private:
    void add(void)
    {
        while(true)
        {
            uint32_t a;
            uint32_t c;
            uint32_t sum;

            fifo_a_in.read(a);
            fifo_c_in.read(c);
            sum = a + c;

            fifo_b_out.write(sum);
        }
    }
};

SC_MODULE(spliter)
{
    public:
    sc_fifo_out<uint32_t> fifo_a_out;
    sc_fifo_in<uint32_t> fifo_b_in;
    sc_fifo_out<uint32_t> fifo_d_out;
    sc_out<uint32_t> e_out;

    SC_CTOR(spliter) : fifo_a_out("fifo_a_out"), fifo_b_in("fifo_b_in"), fifo_d_out("fifo_d_out"), e_out("e_out")
    {
        SC_THREAD(split);
    }

    private:
    uint32_t counter;
    void split(void)
    {
        while(true)
        {
            uint32_t b;

            fifo_b_in.read(b);

            fifo_a_out.write(b);
            fifo_d_out.write(b);
            e_out.write(b);

            if (counter <= 10)
            {
                counter++;
                std::cout<<"Output: "<< b <<std::endl;
            }
            else
            {
                wait(); // stop simulation
            }
        }
    }
};

SC_MODULE(delayer)
{
    public:
    sc_fifo_out<uint32_t> fifo_c_out;
    sc_fifo_in<uint32_t> fifo_d_in;

    SC_CTOR(delayer) : fifo_c_out("fifo_c_out"), fifo_d_in("fifo_d_in")
    {
        SC_THREAD(delay);
    }

    private:
    void delay(void)
    {
        while(true)
        {
            uint32_t d;

            wait(10,SC_NS);

            fifo_d_in.read(d);
            fifo_c_out.write(d);
        }
    }
};


SC_MODULE(kpn)
{
    private:
    sc_fifo<uint32_t> fifo_a;
    sc_fifo<uint32_t> fifo_b;
    sc_fifo<uint32_t> fifo_c;
    sc_fifo<uint32_t> fifo_d;

    // whats the difference?
    // sc_fifo<unsigned int> fifo_a(10);
    // and calling constructor with parameter (10)

    adder add;
    delayer delay;
    spliter split;

    sc_signal<uint32_t> e_out;

    public:
    SC_CTOR(kpn) : fifo_a("fifo_a", 10), fifo_b("fifo_b", 10), fifo_c("fifo_c", 10), fifo_d("fifo_d", 10), add("add"), delay("delay"), split("split")
    {
        add.fifo_a_in.bind(fifo_a);
        add.fifo_c_in.bind(fifo_c);
        add.fifo_b_out.bind(fifo_b);

        delay.fifo_d_in.bind(fifo_d);
        delay.fifo_c_out.bind(fifo_c);

        split.fifo_b_in.bind(fifo_b);
        split.fifo_a_out.bind(fifo_a);
        split.fifo_d_out.bind(fifo_d);
        split.e_out.bind(e_out);

        uint32_t initial_b = 1;
        fifo_b.write(initial_b);

        uint32_t initial_c = 0;
        fifo_c.write(initial_c);
    }
};

#endif // KPN_H
