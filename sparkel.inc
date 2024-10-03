select * from purchases;

select * from vendor_invoice;

-- Step 1: Alter the table to add a new column for the converted date
ALTER TABLE purchases ADD COLUMN new_PODate DATE;
SET SQL_SAFE_UPDATES = 0;
-- Step 2: Update the new column with converted date values
UPDATE purchases
SET new_PODate = STR_TO_DATE(PODate, '%d/%m/%Y');

-- Drop the old sales_date column
ALTER TABLE purchases
DROP COLUMN PODate;

-- Rename the new_sales_date column to sales_date
ALTER TABLE purchases 
CHANGE COLUMN new_PODate  PODate DATE;

show columns from purchases ;
describe PODate;

ALTER TABLE purchases ADD COLUMN new_ReceivingDate DATE;

-- Step 2: Update the new column with converted date values
UPDATE purchases
SET new_ReceivingDate = STR_TO_DATE(ReceivingDate, '%d/%m/%Y');

-- Drop the old sales_date column
ALTER TABLE purchases
DROP COLUMN ReceivingDate;

-- Rename the new_sales_date column to sales_date
ALTER TABLE purchases 
CHANGE COLUMN new_ReceivingDate  ReceivingDate DATE;

show columns from purchases ;
describe PODate;

ALTER TABLE purchases ADD COLUMN new_InvoiceDate DATE;

-- Step 2: Update the new column with converted date values
UPDATE purchases
SET new_InvoiceDate = STR_TO_DATE(InvoiceDate, '%d/%m/%Y');

-- Drop the old sales_date column
ALTER TABLE purchases
DROP COLUMN InvoiceDate;

-- Rename the new_sales_date column to sales_date
ALTER TABLE purchases 
CHANGE COLUMN new_InvoiceDate  InvoiceDate DATE;

ALTER TABLE purchases ADD COLUMN new_PayDate DATE;
-- Step 2: Update the new column with converted date values
UPDATE purchases
SET new_PayDate = STR_TO_DATE(PayDate, '%d/%m/%Y');
-- Drop the old sales_date column
ALTER TABLE purchases
DROP COLUMN PayDate;
-- Rename the new_sales_date column to sales_date
ALTER TABLE purchases 
CHANGE COLUMN new_PayDate  PayDate DATE;

-- checking for null 
select * from purchases
where InventoryId is null
or Product_ID is null
or Product_Name is null
or Size is null
or VendorNumber is null
or PONumber is null
or PurchasePrice is null
or Quantity is null
or Classification is null 
or PODate is null
or ReceivingDate is null
or InvoiceDate is null 
or PayDate is null;

select * from vendor_invoice
where VendorNumber is null
or Vendor_Email is null
or VendorName is null;

-- checking for duplicates
select  VendorNumber,VendorName, Vendor_Email, count(*)
from vendor_invoice
group by VendorNumber,VendorName, Vendor_Email
having count(*) >1;

-- checking date range in the dataset 
select min(PODate) as mindate,
       max(PODate) as maxdate
from purchases;

select min(ReceivingDate) as mindate,
       max(ReceivingDate) as maxdate
from purchases;

select min(InvoiceDate) as mindate,
       max(InvoiceDate) as maxdate
from purchases;

select min(PayDate) as mindate,
       max(PayDate) as maxdate
from purchases;


select avg(PurchasePrice)
from purchases;

select Classification, count(*)
from purchases
group by Classification;

-- identify top performing suppliers based on purchase volume, frequency of purchase, and amount spent 
select v.VendorName,
       v.Vendor_Email,
       sum(p.Quantity * p.PurchasePrice) as amount_spent,
       sum(p.Quantity) as purchase_volume,
       count(distinct p.PONumber) as purchase_frequency
from vendor_invoice as v
inner join purchases as p
on v.VendorNumber = p.VendorNumber
group by v.VendorName, v.Vendor_Email
having sum(p.Quantity * p.PurchasePrice) 
order by amount_spent desc, purchase_volume desc, purchase_frequency desc;

-- top 10 wine  suppliers 
select v.VendorName,
       v.Vendor_Email,
       sum(p.Quantity) as quantity_purchased
       from vendor_invoice as v
inner join purchases as p
on v.VendorNumber = p.VendorNumber
where p.Classification = 2
group by v.VendorName, v.Vendor_Email
order by quantity_purchased desc
limit 10;

-- top 10 spirit suppliers 
select v.VendorName,
       v.Vendor_Email,
       sum(p.Quantity) as quantity_purchased
       from vendor_invoice as v
inner join purchases as p
on v.VendorNumber = p.VendorNumber
where p.Classification = 1
group by v.VendorName, v.Vendor_Email
order by quantity_purchased desc
limit 10;

-- function to return top vendors 

CREATE TABLE vendor_purchases (
    VendorID INT,
    VendorName VARCHAR(255),
    VendorEmail VARCHAR(255),
    PurchaseAmount DECIMAL(10, 2),
    PurchaseDate DATE,
    PurchaseID INT AUTO_INCREMENT PRIMARY KEY
);

DELIMITER //
BEGIN
select v.VendorName,
       v.Vendor_Email,
       sum(p.Quantity * p.PurchasePrice) as amount_spent,
       sum(p.Quantity) as purchase_volume,
       count(distinct p.PONumber) as purchase_frequency
from vendor_invoice as v
inner join purchases as p
on v.VendorNumber = p.VendorNumber
group by v.VendorName, v.Vendor_Email
having sum(p.Quantity * p.PurchasePrice) 
order by amount_spent desc, purchase_volume desc, purchase_frequency desc;

END //

DELIMITER ;






