SELECT p.productline,
       Round(Avg(o.shippeddate - o.orderdate), 2) average_time
FROM   products p
       natural JOIN orderdetails od
       natural JOIN orders o
GROUP  BY p.productline
ORDER  BY average_time DESC;