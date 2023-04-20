SELECT consulta.country,
       Count(*) num_offices
FROM   (SELECT o.country country,
               o.officecode
        FROM   offices o
        EXCEPT
        (SELECT op.country,
                op.oc
         FROM   (SELECT o.country            country,
                        o.officecode         oc,
                        Count(p.checknumber) pagos
                 FROM   offices o
                        natural JOIN employees e
                        JOIN customers c
                          ON e.employeenumber = c.salesrepemployeenumber
                        natural JOIN payments p
                 WHERE  Extract(year FROM p.paymentdate) = '2003'
                 GROUP  BY o.country,
                           o.officecode) AS op)) AS consulta
GROUP  BY consulta.country
ORDER  BY Count(*) DESC;