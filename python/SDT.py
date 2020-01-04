from enum import Enum


class SDTState(Enum):
	SDT_ERROR=1
	SDT_OK=2
	SDT_VALUE=3


class SDTInformation:
	State=0
	Value=0.0
	Error=0.0
	def __str__(self):
		return "State: "+str(self.State)+" Value: "+str(self.Value)+" Error: "+str(self.Error)
	def __init__(self,Tstate,Tvalue,Terror):
		self.State=Tstate
		self.Value=float(Tvalue)
		self.Error=float(Terror)



class SDTCoordinates:
	X=0.0
	Y=0.0
	def __str__(self):
		return "X: "+str(self.X)+" Y: "+str(self.Y)+"\n"
	def __repr__(self):
		return (self)
	def __init__(self,TX,TY):
		self.X=float(TX)
		self.Y=float(TY)
	def SetX(self):
		return str(X)+" "

	





class SDT:
	className       ="Swinging Door"
	ClassContext    ="Swing Door::"
	Error 			=0.0 #float
	Init  			=False#bool
	FirstTime 		=True #bool
	TimeLine 		=0.0 #float
	MinimumTime 	=0.0 #float
	MaximumTime 	=0.0#float
	SwingValue 		=SDTCoordinates(0,0)# class SDTCoordinates
	LastPoint   	=SDTCoordinates(0,0)# class SDTCoordinates
	MaxSlopeUpper	=0.0#float
	MinSlopeLower 	=0.0 #float
	MaxCoordenates 	=SDTCoordinates(0,0)# class SDTCoordinates
	MinCoordenates 	=SDTCoordinates(0,0)# class SDTCoordinate
	#class 
	def First(self,Value):
		Value=float(Value)
		self.MaxSlopeUpper=self.UPPER_SLOPE(Value)
		self.MinSlopeLower=self.LOWER_SLOPE(Value)
		self.LastPoint=SDTCoordinates(self.TimeLine,Value)
		#print("MAX: "+str(self.MaxSlopeUpper)+" MIN: "+str(self.MinSlopeLower))
	def begin(self,TError,TMaxTime,TMinTime,TValue):
		self.Error=float(TError)
		self.MaximumTime=float(TMaxTime)
		self.MinimumTime=float(TMinTime)
		self.SwingValue=SDTCoordinates(0.0,float(TValue))
		self.TimeLine=float(0.0)
		self.FirstTime =True
		self.Init=True
		return SDTInformation(SDTState.SDT_VALUE,self.SwingValue.Y,self.Error)
	'''
	def ended(self):
	
		ValueA=float(self.LastPoint.Y)+(self.Error/2.0)
		ValueB=float(self.LastPoint.Y)-(self.Error/2.0)
		self.TimeLine=self.TimeLine+1.0	
		TemporalA= self.UPPER_SLOPE(ValueA)
		ChangedA=False
		if(TemporalA>self.MaxSlopeUpper):
			ChangedA=True
			if(TemporalA>self.MinSlopeLower):
				self.MaxSlopeUpper=TemporalA
				self.SwingValue=SDTCoordinates(self.UPPER_TIME(ValueA),self.UPPER_NEW_VALUE(ValueA))
				return SDTInformation(SDTState.SDT_VALUE,self.SwingValue.Y,self.Error)
		Temporal=self.LOWER_SLOPE(ValueB)
		ChangedB=False
		if(Temporal<self.MinSlopeLower):	
			ChangedB=True
			if(self.MaxSlopeUpper>self.MinSlopeLower):
				self.MinSlopeLower=Temporal
				self.SwingValue=SDTCoordinates(self.LOWER_TIME(ValueB),self.LOWER_NEW_VALUE(ValueB))
				return SDTInformation(SDTState.SDT_VALUE,self.SwingValue.Y,self.Error)

		return SDTInformation(SDTState.SDT_VALUE,self.LastPoint.Y,self.Error)
	'''
	def ended(self):
		MaxPoint= (self.MaxSlopeUpper*(self.TimeLine-self.SwingValue.X)+ self.UPPER_PIVOT())
		MinPoint= (self.MinSlopeLower*(self.TimeLine-self.SwingValue.X)+ self.LOWER_PIVOT())
		Value=(MaxPoint+MinPoint)/2.0
		return SDTInformation(SDTState.SDT_VALUE,Value,self.Error)



	def check(self,Value):
		Value=float(Value)
		self.TimeLine=self.TimeLine+1.0
		if(self.Init):
			if(self.FirstTime):
				self.First(Value)
				self.FirstTime =False;
				self.LastPoint=SDTCoordinates(self.TimeLine,Value)
				return SDTInformation(SDTState.SDT_OK,0.0,0.0)
			Temporal= self.UPPER_SLOPE(Value)
			if(Temporal>self.MaxSlopeUpper):
				self.MaxSlopeUpper=Temporal
				if(self.MaxSlopeUpper>self.MinSlopeLower):
					#print("MAX: "+str(self.MaxSlopeUpper)+" MIN: "+str(self.MinSlopeLower))
					#print("SO: "+str(self.OUT_SLOPE(Value))+" TimeLine: "+str(self.TimeLine)+" CX(i+1): "+str(self.UPPER_TIME(Value))+" OY: "+str(self.UPPER_Y(Value)))
					self.SwingValue=SDTCoordinates(self.UPPER_TIME(Value),self.UPPER_NEW_VALUE(Value))
					if(self.TimeLine==self.SwingValue.X):
						self.SwingValue.X=self.SwingValue.X-0.001
						#print("Aqui puede estar el error: "+str(round(self.Error,3)))
					self.First(Value)
					#if(self.Error==2.0):
					#	print("TimeLine: "+str(self.TimeLine))
					return SDTInformation(SDTState.SDT_VALUE,self.SwingValue.Y,self.Error)

			Temporal=self.LOWER_SLOPE(Value)
			if(Temporal<self.MinSlopeLower):
				self.MinSlopeLower=Temporal
				if(self.MaxSlopeUpper>self.MinSlopeLower):
					#print("MAX: "+str(self.MaxSlopeUpper)+" MIN: "+str(self.MinSlopeLower))
					#print("SO: "+str(self.OUT_SLOPE(Value))+" TimeLine: "+str(self.TimeLine)+" CX(i+1): "+str(self.LOWER_TIME(Value))+" OY: "+str(self.LOWER_Y(Value)))
					self.SwingValue=SDTCoordinates(self.LOWER_TIME(Value),self.LOWER_NEW_VALUE(Value))
					if(self.TimeLine==self.SwingValue.X):
						self.SwingValue.X=self.SwingValue.X-0.001
						#print("Aqui puede estar el error: "+str(self.Error))
					self.First(Value)
					#if(self.Error==2.0):
					#	print("TimeLine: "+str(self.TimeLine))
					return SDTInformation(SDTState.SDT_VALUE,self.SwingValue.Y,self.Error)

			#print("MAX: "+str(self.MaxSlopeUpper)+" MIN: "+str(self.MinSlopeLower))
			self.LastPoint=SDTCoordinates(self.TimeLine,Value)
			return SDTInformation(SDTState.SDT_OK,0.0,self.Error)

		else: 
			#print("The SwingDoor is disabled")
			return SDTInformation(SDTState.SDT_VALUE,self.Value,0.0)
	#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Formulas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	def UPPER_PIVOT(self):
		return self.SwingValue.Y+self.Error
	def LOWER_PIVOT(self):
		return self.SwingValue.Y-self.Error
	#Slope calculation formula
	def UPPER_SLOPE(self,Value):
		return (Value-self.UPPER_PIVOT())/(self.TimeLine-self.SwingValue.X)
	def LOWER_SLOPE(self,Value):
		return (Value-self.LOWER_PIVOT())/(self.TimeLine-self.SwingValue.X)
	def OUT_SLOPE(self,Value):
		return (Value-self.LastPoint.Y)/(self.TimeLine-self.LastPoint.X)
	#New Swing Door time definition
	def UPPER_TIME(self,Value):
		return (self.UPPER_PIVOT()-self.LastPoint.Y + self.OUT_SLOPE(Value)*self.LastPoint.X- self.MinSlopeLower*self.SwingValue.X)/(self.OUT_SLOPE(Value)-self.MinSlopeLower)
	def LOWER_TIME(self,Value):
		return (self.LOWER_PIVOT()-self.LastPoint.Y + self.OUT_SLOPE(Value)*self.LastPoint.X- self.MaxSlopeUpper*self.SwingValue.X)/(self.OUT_SLOPE(Value)-self.MaxSlopeUpper)
	#
	def UPPER_Y(self,Value):
		return self.UPPER_PIVOT()+self.MinSlopeLower*(self.UPPER_TIME(Value)-self.SwingValue.X)
	def LOWER_Y(self,Value):
		return self.LOWER_PIVOT()+self.MaxSlopeUpper*(self.LOWER_TIME(Value)-self.SwingValue.X)
    #New Swing Door value defined
	def UPPER_NEW_VALUE(self,Value):
		return (self.UPPER_Y(Value)-(self.Error/2.0))
	def LOWER_NEW_VALUE(self,Value):
		return (self.LOWER_Y(Value)+(self.Error/2.0))
	def EQUAL_SLOPE(self):
		return (self.MinCoordenates.Y+self.MaxCoordenates.Y-2*self.SwingValue.Y)/(self.MinCoordenates.X+self.MaxCoordenates.X-2.0*self.SwingValue.X)
	def NEW_ERROR(self):
		return (abs(self.MaxCoordenates.Y-self.MinCoordenates.Y)-self.EQUAL_SLOPE()*(self.MaxCoordenates.X-self.MinCoordenates.X))/2.0







def DescompressionSDT(Value):
	Quantity=len(Value)
	#print(str(Quantity))
	Array=[]
	for Number in range(0,Quantity-1):
		Slope=SLOPE(Value[Number+1],Value[Number])
		for point in range(int(Value[Number].X),int(Value[Number+1].X)):
			Array.append(float(POINT(float(point),float(Value[Number].X),Slope,float(Value[Number].Y))))
	Array.append(Value[Quantity-1].Y)
	#print("Last: "+str(Value[Quantity-1].X)+" Quantity: "+str(Quantity) +" Descompression: "+ str(test))
	return Array
def SLOPE(B,A):
	return (B.Y-A.Y)/(B.X-A.X)

def POINT(X2,X1,M,Y):
	return ((X2-X1)*M+Y)


def ERROR(A,B):
	Quantity=len(A)
	Value=0.0
	for a in range(0,Quantity):
		Value=Value+((A[a]-B[a])**2)/float(Quantity)
	return Value

def RGE(A,B):
	Quantity=len(A)
	ValueF=0.0
	Value=0.0
	ValueB=0.0
	for a in range(0,Quantity):
		Value=Value+((float(A[a])-float(B[a]))**2.0)
		ValueB=ValueB+(float(A[a])**2.0)
	ValueF=Value/ValueB
	return ValueF

def RGAE(A,B):
	Quantity=len(A)
	ValueF=0.0
	Value=0.0
	ValueB=0.0
	for a in range(0,Quantity):
		Value=Value+abs((float(A[a])-float(B[a])))
		ValueB=ValueB+abs(float(A[a]))
	ValueF=(Value/ValueB)
	return ValueF

def RGLE(A,B):
	Quantity=len(A)
	ValueF=0.0
	Value=0.0
	ValueB=0.0
	for a in range(0,Quantity):
		result=(float(A[a])-float(B[a]))
		if result>=0.0:
			Value=Value+result
		else:
			Value=Value-result
		ValueB=ValueB+float(A[a])
	ValueF=Value/ValueB
	return ValueF


def  PERCENTAGE(A,B):
	return (float(len(A)-len(B))/float(len(A)))

def CE(A,B):
	Value=0.0
	Quantity=len(A)
	for a in range(0,Quantity):
		Value=Value+((A[a]-B[a])**2)/Quantity
	return Value

def CR(A,B):
	QUANTITY_A=len(A)
	QUANTITY_B=len(B)
	return QUANTITY_A/QUANTITY_B

def OPTIMIZATEError(A,B):
	Max=0.0
	for Variable in A:
		if(Max<Variable):
			Max=Variable
	AFinal=[]
	for Variable in A:
		AFinal.append(Variable/Max)
	Max=0.0
	for Variable in B:
		if(Max<Variable):
			Max=Variable
	Result=[]
	count=0
	for Variable in B:
		Result.append(AFinal[count]-Variable/Max)
		count=count+1
	return Result


def ErrorCriteria(Compression, Criteria):
	Max=0.0
	for value in Compression:
		if(value<=Criteria and Max<value):
			Max=value
	return Max

def BetterCase(Desviation,Error,Compression,Optimization):
	n=0
	MaxValue=-100;
	MaxValueVector=[]
	for Sample in Optimization:
		if(Sample>MaxValue):
			MaxValue=Sample
			MaxValueVector=[]
			MaxValueVector.append([Desviation[n],Error[n],Compression[n],Optimization[n]])
		else: 
			if(Sample==MaxValue):
				MaxValueVector.append([Desviation[n],Error[n],Compression[n],Optimization[n]])
		n=n+1;
	size=len(MaxValueVector)
	return MaxValueVector[size-1]



def OPTIMIZATEPercentage(Rate,Error):

	Quantity=len(Rate)
	Result=[]
	for i in range(0,Quantity):
		Result.append((Rate[i]-Error[i]))
	return Result



def SelectionCriterial(Rate,Error):

	Quantity=len(Rate)
	Result=[]
	armonic=0.0
	success=0.0
	for i in range(0,Quantity):
		success=1.0-Error[i]
		if(success<0.0):
			success=0.0
		armonic=(2*Rate[i]*(success)/(Rate[i]+success))
		Result.append(armonic)
	return Result

def ScalarSelectionCriterial(Rate,Error):
	success=1.0-Error
	armonic=(2*Rate*(success)/(Rate+success))
	return armonic

def SelectionCriterialwithCriterialError(Rate,Error,Criterial_error):

	Quantity=len(Rate)
	Result=[]
	armonic=0.0
	success=0.0
	for i in range(0,Quantity):
		success=1.0-Error[i]
		if(success<0.0 or success<(1.0-Criterial_error)):
			success=0.0
		armonic=(2*Rate[i]*(success)/(Rate[i]+success))
		Result.append(armonic)
	return Result







