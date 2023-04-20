SELECT c.customernumber,
       c.customername,
       Sum(p.amount)
FROM   customers c
       natural JOIN payments p
WHERE  c.customernumber IN (SELECT c.customernumber
                            FROM   customers c
                                   natural JOIN orders o
                                   natural JOIN orderdetails od
                                   natural JOIN products p
                            WHERE  p.productname = '1940 Ford Pickup Truck')
GROUP  BY c.customernumber
ORDER  BY Sum(p.amount) DESC;