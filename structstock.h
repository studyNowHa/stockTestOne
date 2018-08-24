#ifndef STRUCTSTOCK_H
#define STRUCTSTOCK_H
#include <QString>

class structStock
{
public:
    structStock();
    structStock(QString str);
    QString stock_name;                 //股票名字
    QString stock_num;                  //股票代码
    QString stock_price;                //当前价格
    QString stock_yest_price;           //昨收
    QString stock_today_price;          //今开
    QString stock_volume;               //成交量（手）
    QString stock_volume_history;       //历史成交量
    QString stock_total_money;          //总成交额
    QString stock_sell_volume;          //卖一量（手）
    QString stock_buy_volume;           //买一量（手）
    QString stock_uplift;               //涨幅
    QString stock_uplift_percent;       //涨跌%
    QString stock_limit_up_price;       //涨停价
    QString stock_limit_down_price;     //跌停价
    QString stock_time;                 //时间
    bool    stock_get_data_state = false;       //获取数据的有效状态
    bool    stock_state = false;                //股票是否停牌
    float stock_sell_total_money;     //股票卖一总金额
    float stock_buy_total_money;     //股票买一总金额
};

#endif // STRUCTSTOCK_H
