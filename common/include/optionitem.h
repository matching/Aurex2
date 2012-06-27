// -*-c++-*-
// $Id: optionitem.h,v 1.11 2004/11/01 11:12:16 fumi Exp $

#ifndef OPTIONITEM_H
#define OPTIONITEM_H

class OptionItem
{
  private:
	char *key;

  public:
	OptionItem(char const *key);
	virtual ~OptionItem(void);

	char const *getKeyString(void);

// parse できたら true かえす
	virtual bool setValueString(char const *value) = 0;
	virtual void getValueString(char *out) = 0;
};

class OptionBool : public OptionItem
{
  protected:
	bool value;

  public:
	OptionBool(char const *key, bool init);
	~OptionBool(void);

	bool setValueString(char const *value);
	void getValueString(char *out);

	bool getBoolValue(void);
	void setBoolValue(bool v);
};

class OptionInt : public OptionItem
{
  protected:
	int value;

  public:
	OptionInt(char const *key, int init);
	~OptionInt(void);

	bool setValueString(char const *value);
	void getValueString(char *out);


	int  getIntValue(void);
	void setIntValue(int v);
};

class OptionString : public OptionItem
{
  protected:
	char *value;

  public:
	OptionString(char const *key, char const *init);
	~OptionString(void);

	bool setValueString(char const *value);
	void getValueString(char *out);

	void setStringValue(char const *v);
	char const *getStringValue(void);
};

#endif
