SELECT o1.officecode,
       Sum(o3.quantityordered) AS number_of_product_sales
FROM   offices o1
       NATURAL JOIN employees e
       JOIN customers c
         ON e.employeenumber = c.salesrepemployeenumber
       NATURAL JOIN orders o2
       NATURAL JOIN orderdetails o3
GROUP  BY o1.officecode
ORDER  BY number_of_product_sales DESC
LIMIT  1;