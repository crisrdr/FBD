SELECT p1.productcode,
       p2.productcode,
       Count(o2) AS carros
FROM   products p1
       natural JOIN orderdetails o1
       natural JOIN orders o2,
       products p2
       natural JOIN orderdetails o
       natural JOIN orders o3
WHERE  p1.productcode != p2.productcode
       AND o1.ordernumber = o.ordernumber
       AND o2.ordernumber = o3.ordernumber
       AND p1.productcode < p2.productcode
GROUP  BY p1.productcode,
          p2.productcode
ORDER  BY carros DESC; 